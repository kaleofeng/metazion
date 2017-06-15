#include "Metazion/Net/NetworkService.hpp"

#include <Metazion/Share/Utility/Random.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

NetworkService::NetworkService() {}

NetworkService::~NetworkService() {}

bool NetworkService::Initialize(int socketCapacity, int ioThreadNumber) {
    m_socketCapacity = socketCapacity;
    m_ioThreadNumber = ioThreadNumber;

    const auto ret = m_socketService.Initialize(m_socketCapacity, m_ioThreadNumber);
    if (!ret) {
        return false;
    }

    m_socketCtrlList = new SocketCtrl[m_socketCapacity];

    m_ioThreadList = new IoThread*[m_ioThreadNumber];
    for (auto index = 0; index < m_ioThreadNumber; ++index) {
        auto& ioThread = m_ioThreadList[index];
        ioThread = new IoThread();
        ioThread->Initialize(this, index);
        ioThread->Run();
    }

    m_maintenanceThread = new MaintenanceThread();
    m_maintenanceThread->Init(this);
    m_maintenanceThread->Run();

    m_sockets = new Socket*[m_socketCapacity] { nullptr };
    m_socketArray.Attach(m_sockets, m_socketCapacity, 0);
    return true;
}

void NetworkService::Finalize() {
    m_socketArray.Detach();
    SafeDeleteArray(m_sockets);

    m_maintenanceThread->Finalize();
    SafeDelete(m_maintenanceThread);

    for (auto index = 0; index < m_ioThreadNumber; ++index) {
        auto& ioThread = m_ioThreadList[index];
        ioThread->Finalize();
        SafeDelete(ioThread);
    }
    SafeDeleteArray(m_ioThreadList);

    for (auto index = 0; index < m_socketCapacity; ++index) {
        auto& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        socketCtrl.m_active
            ? socketCtrl.m_socket->Close()
            : socketCtrl.m_socket->DetachSockId();

        socketCtrl.m_socket->Destory();
        socketCtrl.m_socket = nullptr;
    }
    SafeDeleteArray(m_socketCtrlList);

    m_socketService.Finalize();
}

void NetworkService::Tick() {
    const auto size = AcquireSockets([](Socket* socket) {
        return socket->GetType() != SOCKET_TCP_LISTEN;
    }, m_socketArray);

    for (auto i = 0; i < size; ++i) {
        auto socket = m_socketArray[i];
        MZ_ASSERT_TRUE(!NS_MZ::IsNull(socket));
        socket->Dispatch();
    }

    ReleaseSockets(m_socketArray);
}

bool NetworkService::Manage(Socket* socket) {
    MZ_ASSERT_TRUE(!IsNull(socket));

    if (m_socketNumber >= m_socketCapacity) {
        return false;
    }

    Lock();
    const auto index = GetVacantIndex();
    AddSocketCtrl(index, socket);
    Unlock();

    socket->SetIndex(index);
    socket->SetNetworkService(this);
    socket->OnAttached();
    return true;
}

int NetworkService::AcquireSockets(std::function<bool(Socket*)> socketFilter, SocketArray_t& socketArray) {
    socketArray.Clear();

    Lock();
    const auto socketCapacity = GetSocketCapacity();
    for (auto index = 0; index < socketCapacity; ++index) {
        auto socket = GetSocket(index);
        if (IsNull(socket)) {
            continue;
        }
        if (!socketFilter(socket)) {
            continue;
        }

        socket->Retain();
        socketArray.Append(socket);
    }
    Unlock();

    return socketArray.GetSize();
}

void NetworkService::ReleaseSockets(SocketArray_t& socketArray) {
    const auto size = socketArray.GetSize();
    for (auto index = 0; index < size; ++index) {
        auto socket = socketArray[index];
        MZ_ASSERT_TRUE(!IsNull(socket));
        socket->Release();
    }
}

void NetworkService::AddSocketCtrl(int index, Socket* socket) {
    if (index >= 0 && index < m_socketCapacity) {
        m_socketCtrlList[index].m_socket = socket;
        m_socketCtrlList[index].m_active = false;
        ++m_socketNumber;
    }
}

void NetworkService::RemoveSocketCtrl(int index) {
    if (index >= 0 && index < m_socketCapacity) {
        m_socketCtrlList[index].m_socket = nullptr;
        m_socketCtrlList[index].m_active = false;
        --m_socketNumber;
    }
}

int NetworkService::GetVacantIndex() const {
    static NS_SHARE::Random s_random;
    auto count = 0;
    while (count++ < 3) {
        auto index = s_random.GetRangeInt(0, m_socketCapacity - 1);
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    for (auto index = 0; index < m_socketCapacity; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    MZ_ASSERT_TRUE(false);
    return -1;
}

void NetworkService::MarkSocketActive(int index) {
    MZ_ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    MZ_ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    MZ_ASSERT_TRUE(!m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = true;

    auto socket = m_socketCtrlList[index].m_socket;
    socket->Prepare();

    if (!m_socketService.Associate(socket)) {
        socket->DetachSockId();
        return;
    }

    socket->Start();
}

void NetworkService::MarkSocketClosed(int index) {
    MZ_ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    MZ_ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    MZ_ASSERT_TRUE(m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = false;
}

DECL_NAMESPACE_MZ_NET_END
