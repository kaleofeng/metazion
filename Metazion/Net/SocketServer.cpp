#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SocketServer::SocketServer()
    : m_socketCapacity(0)
    , m_socketNumber(0)
    , m_socketCtrlList(nullptr)
    , m_ioThreadNumber(0)
    , m_ioThreadList(nullptr)
    , m_maintenanceThread(nullptr) {}

SocketServer::~SocketServer() {}

bool SocketServer::Initialize(int socketCapacity, int ioThreadNumber) {
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
    return true;
}

void SocketServer::Finalize() {
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

    m_socketService.Finalize();
}

bool SocketServer::Attach(Socket* socket) {
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

void SocketServer::AddSocketCtrl(int index, Socket* socket) {
    m_socketCtrlList[index].m_socket = socket;
    m_socketCtrlList[index].m_active = false;
    ++m_socketNumber;
}

void SocketServer::RemoveSocketCtrl(int index) {
    m_socketCtrlList[index].m_socket = nullptr;
    m_socketCtrlList[index].m_active = false;
    --m_socketNumber;
}

int SocketServer::GetVacantIndex() const {
    for (auto index = 0; index < m_socketCapacity; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    ASSERT_TRUE(false);
    return -1;
}

void SocketServer::MarkSocketActive(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(!m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = true;

    auto socket = m_socketCtrlList[index].m_socket;
    if (!m_socketService.Associate(socket)) {
        socket->DetachSockId();
        return;
    }

    socket->Start();
}

void SocketServer::MarkSocketClosed(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(m_socketCtrlList[index].m_active);

    auto& socketCtrl = m_socketCtrlList[index];
    socketCtrl.m_active = false;
}

DECL_NAMESPACE_MZ_NET_END
