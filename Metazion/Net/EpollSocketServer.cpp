#include "Metazion/Net/EpollSocketServer.hpp"

#include <Metazion/Share/Time/Time.hpp>
#include <Metazion/Share/Utility/Random.hpp>

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
    for (int index = 0; index < m_ioThreadNumber; ++index) {
        int& epollfd = m_epollfdList[index];
        const int socketCount = GetSocketCount(index);
        epollfd = ::epoll_create(socketCount);
        if (epollfd < 0) {
            return false;
        }
    }

    m_epollEventList = new struct epoll_event[m_socketCapacity];
    ::memset(m_epollEventList, 0, sizeof(struct epoll_event) * m_socketCapacity);

    m_socketCtrlList = new SocketCtrl[m_socketCapacity];

    m_ioThreadList = new EpollIoThread*[m_ioThreadNumber];
    for (int index = 0; index < m_ioThreadNumber; ++index) {
        EpollIoThread*& ioThread = m_ioThreadList[index];
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
        EpollIoThread*& ioThread = m_ioThreadList[index];
        ioThread->Finalize();
        SafeDelete(ioThread);
    }
    SafeDeleteArray(m_ioThreadList);

    for (int index = 0; index < m_socketCapacity; ++index) {
        SocketCtrl& socketInfo = m_socketCtrlList[index];
        if (IsNull(socketInfo.m_socket)) {
            continue;
        }

        if (socketInfo.m_active) {
            socketInfo.m_socket->Close();
        }
        else {
            socketInfo.m_socket->CloseSockId();
        }
        socketInfo.m_socket->Destory();
        socketInfo.m_socket = nullptr;
    }
    SafeDeleteArray(m_socketCtrlList);

    SafeDeleteArray(m_epollEventList);

    for (int index = 0; index < m_ioThreadNumber; ++index) {
        int& epollfd = m_epollfdList[index];
        if (epollfd >= 0) {
            ::close(epollfd);
        }
    }
    SafeDeleteArray(m_epollfdList);
}

bool EpollSocketServer::Attach(Socket* socket) {
    EpollSocket* epollSocket = static_cast<EpollSocket*>(socket);
    ASSERT_TRUE(!IsNull(epollSocket));

    if (!CanAttachMore()) {
        return false;
    }

    Lock();
    const int index = GetVacantIndex();
    AddSocketCtrl(index, epollSocket);
    Unlock();

    epollSocket->SetIndex(index);
    epollSocket->SetSocketServer(this);
    epollSocket->OnAttached();
    return true;
}

bool EpollSocketServer::CanAttachMore() const {
    return m_socketCount < m_socketCapacity;
}

void EpollSocketServer::MarkSocketActive(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(!m_socketCtrlList[index].m_active);

    SocketCtrl& socketInfo = m_socketCtrlList[index];
    socketInfo.m_active = true;

    EpollSocket* epollSocket = m_socketCtrlList[index].m_socket;
    if (!AssociateWithEpoll(epollSocket)) {
        epollSocket->CloseSockId();
        return;
    }

    epollSocket->Rework();
}

void EpollSocketServer::MarkSocketClosed(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(m_socketCtrlList[index].m_active);

    SocketCtrl& socketInfo = m_socketCtrlList[index];
    socketInfo.m_active = false;
}

EpollSocketServer::SocketCtrl& EpollSocketServer::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

void EpollSocketServer::AddSocketCtrl(int index, EpollSocket* socket) {
    m_socketCtrlList[index].m_socket = socket;
    m_socketCtrlList[index].m_active = false;
    ++m_socketCount;
}

void EpollSocketServer::RemoveSocketCtrl(int index) {
    m_socketCtrlList[index].m_socket = nullptr;
    m_socketCtrlList[index].m_active = false;
    --m_socketCount;
}

int EpollSocketServer::GetEpollfd(int threadIndex) const {
    return m_epollfdList[threadIndex];
}

struct epoll_event& EpollSocketServer::GetEpollEvent(int index) {
    return m_epollEventList[index];
}

bool EpollSocketServer::AssociateWithEpoll(EpollSocket* socket) {
    const SockId_t& sockId = socket->GetSockId();
    const int socketIndex = socket->GetIndex();
    const int threadIndex = GetThreadIndex(socketIndex);

    struct epoll_event event;
    event.data.ptr = socket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    const int ret = epoll_ctl(m_epollfdList[threadIndex], EPOLL_CTL_ADD, sockId, &event);
    if (ret == -1) {
        return false;
    }

    return true;
}

int EpollSocketServer::GetVacantIndex() const {
    const int threadIndex = g_random.GetRangeInt(0, m_ioThreadNumber - 1);
    const int startIndex = GetStartIndex(threadIndex);
    for (int index = startIndex; index < m_socketCapacity; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }
    for (int index = 0; index < startIndex; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    ASSERT_TRUE(false);
    return -1;
}

int EpollSocketServer::GetThreadIndex(int socketIndex) {
    const int eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return socketIndex / eachCount;
}

int EpollSocketServer::GetStartIndex(int threadIndex) const {
    const int eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return eachCount * threadIndex;
}

int EpollSocketServer::GetSocketCount(int threadIndex) const {
    const int eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    const int startIndex = eachCount * threadIndex;
    const int restCount = m_socketCapacity - startIndex;
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
    const int startIndex = m_socketServer->GetStartIndex(m_index);
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
    for (int index = 0; index < m_socketCount; ++index) {
        EpollSocketServer::SocketCtrl& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (socketCtrl.m_active) {
            continue;
        }

        socketCtrl.m_socket->Output();
    }
}

void EpollIoThread::ProcessEvents() {
    const int count = ::epoll_wait(m_epollfd, m_eventList, m_socketCount, 10);
    if (0 == count) {
        return;
    }
    else if (count < 0) {
        const int error = GetLastError();
        if (EINTR != error) {
            ::printf("Socket Warning: epoll wait, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
        }
        return;
    }

    for (int index = 0; index < count; ++index)  {
        struct epoll_event& event = m_eventList[index];
        EpollSocket* epollSocket = static_cast<EpollSocket*>(event.data.ptr);

        if (event.events & EPOLLRDHUP) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            epollSocket->Close();
            continue;
        }

        if (event.events & EPOLLIN) {
            epollSocket->Input();
        }

        if (event.events & EPOLLOUT) {
            epollSocket->SetCanOutput(true);
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
        EpollSocketServer::SocketCtrl& socketCtrl = m_socketServer->GetSocketCtrl(index);
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

void EpollMaintenanceThread::ProcessActiveSocket(EpollSocket* epollSocket, int index) {
    if (epollSocket->IsClosed()) {
        m_socketServer->MarkSocketClosed(index);
        return;
    }

    epollSocket->Tick(m_interval);
}

void EpollMaintenanceThread::ProcessClosedSocket(EpollSocket* epollSocket, int index) {
    if (epollSocket->IsActive()) {
        m_socketServer->MarkSocketActive(index);
        return;
    }

    epollSocket->Tick(m_interval);

    if (!epollSocket->IsClosing()) {
        m_socketServer->Lock();
        if (epollSocket->IsClosing()) {
            m_socketServer->Unlock();
            return;
        }

        m_socketServer->RemoveSocketCtrl(index);
        m_socketServer->Unlock();

        epollSocket->OnDetached();
        epollSocket->Destory();
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
