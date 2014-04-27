#include "Metazion/Net/EpollSocketServer.hpp"

#include <Metazion/Share/Time/Time.hpp>
#include <Metazion/Share/Utility/Random.hpp>

#include "Metazion/Net/ListenSocket.hpp"
#include "Metazion/Net/TransmitSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

static NS_SHARE::Random g_random;

EpollSocketServer::EpollSocketServer()
    : m_epollfdList(nullptr)
    , m_epollEventList(nullptr)
    , m_socketCapacity(0)
    , m_socketCount(0)
    , m_socketCtrlList(nullptr)
    , m_ioThreadNumber(0)
    , m_ioThreadList(nullptr)
    , m_maintenanceThread(nullptr) {}

EpollSocketServer::~EpollSocketServer() {}

bool EpollSocketServer::Initialize(int socketCapacity, int ioThreadNumber) {
    m_socketCapacity = socketCapacity;
    m_ioThreadNumber = ioThreadNumber;

    m_epollfdList = new int[m_ioThreadNumber];
    for (auto index = 0; index < m_ioThreadNumber; ++index) {
        auto& epollfd = m_epollfdList[index];
        const auto socketCount = GetSocketCount(index);
        epollfd = ::epoll_create(socketCount);
        if (epollfd < 0) {
            return false;
        }
    }

    m_epollEventList = new struct epoll_event[m_socketCapacity];
    ::memset(m_epollEventList, 0, sizeof(struct epoll_event) * m_socketCapacity);

    m_socketCtrlList = new SocketCtrl[m_socketCapacity];

    m_ioThreadList = new EpollIoThread*[m_ioThreadNumber];
    for (auto index = 0; index < m_ioThreadNumber; ++index) {
        auto& ioThread = m_ioThreadList[index];
        ioThread = new EpollIoThread();
        ioThread->Initialize(this, index);
        ioThread->Run();
    }

    m_maintenanceThread = new EpollMaintenanceThread();
    m_maintenanceThread->Init(this);
    m_maintenanceThread->Run();
    return true;
}

void EpollSocketServer::Finalize() {
    m_maintenanceThread->Finalize();
    SafeDelete(m_maintenanceThread);

    for (int index = 0; index < m_ioThreadNumber; ++index) {
        auto& ioThread = m_ioThreadList[index];
        ioThread->Finalize();
        SafeDelete(ioThread);
    }
    SafeDeleteArray(m_ioThreadList);

    for (int index = 0; index < m_socketCapacity; ++index) {
        auto& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (socketCtrl.m_active) {
            socketCtrl.m_socket->Close();
        }
        else {
            socketCtrl.m_socket->DetachSockId();
        }
        socketCtrl.m_socket->Destory();
        socketCtrl.m_socket = nullptr;
    }
    SafeDeleteArray(m_socketCtrlList);

    SafeDeleteArray(m_epollEventList);

    for (int index = 0; index < m_ioThreadNumber; ++index) {
        auto& epollfd = m_epollfdList[index];
        if (epollfd >= 0) {
            ::close(epollfd);
        }
    }
    SafeDeleteArray(m_epollfdList);
}

bool EpollSocketServer::Attach(Socket* socket) {
    ASSERT_TRUE(!IsNull(socket));

    if (!CanAttachMore()) {
        return false;
    }

    Lock();
    const auto index = GetVacantIndex();
    AddSocketCtrl(index, socket);
    Unlock();

    socket->SetIndex(index);
    socket->SetSocketServer(this);
    socket->OnAttached();
    return true;
}

void EpollSocketServer::MarkSocketActive(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(!m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = true;

    auto socket = m_socketCtrlList[index].m_socket;
    if (!AssociateWithEpoll(socket)) {
        socket->DetachSockId();
        return;
    }

    socket->Start();
}

void EpollSocketServer::MarkSocketClosed(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = false;
}

bool EpollSocketServer::AssociateWithEpoll(Socket* socket) {
    const auto& sockId = socket->GetSockId();
    const auto socketIndex = socket->GetIndex();
    const auto threadIndex = GetThreadIndex(socketIndex);

    struct epoll_event event;
    event.data.ptr = socket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    const auto ret = epoll_ctl(m_epollfdList[threadIndex], EPOLL_CTL_ADD, sockId, &event);
    if (ret == -1) {
        return false;
    }

    return true;
}

void EpollSocketServer::AddSocketCtrl(int index, Socket* socket) {
    m_socketCtrlList[index].m_socket = socket;
    m_socketCtrlList[index].m_active = false;
    ++m_socketCount;
}

void EpollSocketServer::RemoveSocketCtrl(int index) {
    m_socketCtrlList[index].m_socket = nullptr;
    m_socketCtrlList[index].m_active = false;
    --m_socketCount;
}

int EpollSocketServer::GetVacantIndex() const {
    const auto threadIndex = g_random.GetRangeInt(0, m_ioThreadNumber - 1);
    const auto startIndex = GetStartIndex(threadIndex);
    for (auto index = startIndex; index < m_socketCapacity; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }
    for (auto index = 0; index < startIndex; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    ASSERT_TRUE(false);
    return -1;
}

int EpollSocketServer::GetThreadIndex(int socketIndex) {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return socketIndex / eachCount;
}

int EpollSocketServer::GetStartIndex(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return eachCount * threadIndex;
}

int EpollSocketServer::GetSocketCount(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    const auto startIndex = eachCount * threadIndex;
    const auto restCount = m_socketCapacity - startIndex;
    return restCount < eachCount ? restCount : eachCount;
}

EpollIoThread::EpollIoThread()
    : m_socketServer(nullptr)
    , m_index(0)
    , m_epollfd(0)
    , m_eventList(nullptr)
    , m_socketCount(0)
    , m_socketCtrlList(nullptr)
    , m_stopDesired(false) {}

EpollIoThread::~EpollIoThread() {}

void EpollIoThread::Initialize(EpollSocketServer* socketServer, int index) {
    m_socketServer = socketServer;
    m_index = index;
    const auto startIndex = m_socketServer->GetStartIndex(m_index);
    m_epollfd = m_socketServer->GetEpollfd(m_index);
    m_eventList = &m_socketServer->GetEpollEvent(startIndex);
    m_socketCount = m_socketServer->GetSocketCount(m_index);
    m_socketCtrlList = &m_socketServer->GetSocketCtrl(startIndex);
    m_stopDesired = false;
}

void EpollIoThread::Finalize() {
    Stop();
}

void EpollIoThread::Stop() {
    m_stopDesired = true;
    Wait();
}

void EpollIoThread::Execute() {
    while (!m_stopDesired) {
        ProcessSockets();
        ProcessEvents();
    }
}

void EpollIoThread::ProcessSockets() {
    for (auto index = 0; index < m_socketCount; ++index) {
        auto& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (!socketCtrl.m_active) {
            continue;
        }

        socketCtrl.m_socket->GetIoStrategy().Output();
    }
}

void EpollIoThread::ProcessEvents() {
    const auto count = ::epoll_wait(m_epollfd, m_eventList, m_socketCount, 10);
    if (0 == count) {
        return;
    }
    
    if (count < 0) {
        const auto error = GetLastError();
        if (EINTR != error) {
            ::printf("Socket Warning: epoll wait, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
        }
        return;
    }

    for (auto index = 0; index < count; ++index)  {
        auto& event = m_eventList[index];
        auto socket = static_cast<Socket*>(event.data.ptr);

        if (event.events & EPOLLRDHUP) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            socket->Close();
            continue;
        }

        if (event.events & EPOLLIN) {
            socket->GetIoStrategy().Input();
        }

        if (event.events & EPOLLOUT) {
            socket->GetIoStrategy().EnableOutput();
        }
    }
}

EpollMaintenanceThread::EpollMaintenanceThread()
    : m_socketServer(nullptr)
    , m_interval(0)
    , m_stopDesired(false) {}

EpollMaintenanceThread::~EpollMaintenanceThread() {}

void EpollMaintenanceThread::Init(EpollSocketServer* socketServer) {
    ASSERT_TRUE(!IsNull(socketServer));

    m_socketServer = socketServer;
    m_interval = 0;
    m_stopDesired = false;
}

void EpollMaintenanceThread::Finalize() {
    Stop();
}

void EpollMaintenanceThread::Stop() {
    m_stopDesired = true;
    Wait();
}

void EpollMaintenanceThread::Execute() {
    int32_t lastTime = NS_SHARE::GetTickMillisecond();
    int32_t lastTickTime = lastTime;
    while (!m_stopDesired) {
        const int32_t curTime = NS_SHARE::GetTickMillisecond();
        if (curTime - lastTime < 10) {
            MilliSleep(1);
            continue;
        }

        m_interval = curTime - lastTickTime;
        lastTickTime = curTime;

        ProcessSockets();

        lastTime = curTime;
    }
}

void EpollMaintenanceThread::ProcessSockets() {
    const int socketCapacity = m_socketServer->GetSocketCapacity();
    for (int index = 0; index < socketCapacity; ++index) {
        auto& socketCtrl = m_socketServer->GetSocketCtrl(index);
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (socketCtrl.m_active) {
            ProcessActiveSocket(socketCtrl.m_socket, index);
        }
        else {
            ProcessClosedSocket(socketCtrl.m_socket, index);
        }
    }
}

void EpollMaintenanceThread::ProcessActiveSocket(Socket* socket, int index) {
    if (socket->IsClosed()) {
        m_socketServer->MarkSocketClosed(index);
        return;
    }

    socket->Tick(m_interval);
}

void EpollMaintenanceThread::ProcessClosedSocket(Socket* socket, int index) {
    if (socket->IsActive()) {
        m_socketServer->MarkSocketActive(index);
        return;
    }

    socket->Tick(m_interval);

    if (!socket->IsAlive()) {
        m_socketServer->Lock();
        if (socket->IsAlive()) {
            m_socketServer->Unlock();
            return;
        }

        m_socketServer->RemoveSocketCtrl(index);
        m_socketServer->Unlock();

        socket->OnDetached();
        socket->Destory();
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
