#include "Metazion/Net/IocpSocketServer.hpp"

#include <Metazion/Share/Time/Time.hpp>

#if defined(MZ_PLATFORM_WINOWS)

DECL_NAMESPACE_MZ_NET_BEGIN

IocpSocketServer::IocpSocketServer()
    : m_hIocp(nullptr)
    , m_socketCapacity(0)
    , m_socketCount(0)
    , m_socketCtrlList(nullptr)
    , m_ioThreadNumber(0)
    , m_ioThreadList(nullptr)
    , m_maintenanceThread(nullptr) {}

IocpSocketServer::~IocpSocketServer() {}

bool IocpSocketServer::Initialize(int socketCapacity, int ioThreadNumber) {
    m_socketCapacity = socketCapacity;
    m_ioThreadNumber = ioThreadNumber;

    m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, m_ioThreadNumber);
    if (IsNull(m_hIocp)) {
        return false;
    }

    m_socketCtrlList = new SocketCtrl[m_socketCapacity];

    m_ioThreadList = new IocpIoThread*[m_ioThreadNumber];
    for (int index = 0; index < m_ioThreadNumber; ++index) {
        IocpIoThread*& ioThread = m_ioThreadList[index];
        ioThread = new IocpIoThread();
        ioThread->Initialize(this);
        ioThread->Run();
    }

    m_maintenanceThread = new IocpMaintenanceThread();
    m_maintenanceThread->Init(this);
    m_maintenanceThread->Run();
    return true;
}

void IocpSocketServer::Finalize() {
    m_maintenanceThread->Finalize();
    SafeDelete(m_maintenanceThread);

    for (int index = 0; index < m_ioThreadNumber; ++index) {
        IocpIoThread*& ioThread = m_ioThreadList[index];
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
            socketInfo.m_socket->DetachSockId();
        }
        socketInfo.m_socket->Destory();
        socketInfo.m_socket = nullptr;
    }
    SafeDeleteArray(m_socketCtrlList);

    ::CloseHandle(m_hIocp);
}

bool IocpSocketServer::Attach(Socket* socket) {
    IocpSocket* iocpSocket = static_cast<IocpSocket*>(socket);
    ASSERT_TRUE(!IsNull(iocpSocket));

    if (!CanAttachMore()) {
        return false;
    }

    Lock();
    const int index = GetVacantIndex();
    AddSocketCtrl(index, iocpSocket);
    Unlock();

    iocpSocket->SetIndex(index);
    iocpSocket->SetSocketServer(this);
    iocpSocket->OnAttached();
    return true;
}

bool IocpSocketServer::CanAttachMore() const {
    return m_socketCount < m_socketCapacity;
}

void IocpSocketServer::MarkSocketActive(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(!m_socketCtrlList[index].m_active);

    SocketCtrl& socketInfo = m_socketCtrlList[index];
    socketInfo.m_active = true;

    IocpSocket* iocpSocket = m_socketCtrlList[index].m_socket;
    if (!AssociateWithIocp(iocpSocket)) {
        iocpSocket->DetachSockId();
        return;
    }

    iocpSocket->Rework();
}

void IocpSocketServer::MarkSocketClosed(int index) {
    ASSERT_TRUE(index >= 0 && index < m_socketCapacity);
    ASSERT_TRUE(!IsNull(m_socketCtrlList[index].m_socket));
    ASSERT_TRUE(m_socketCtrlList[index].m_active);

    SocketCtrl& socketInfo = m_socketCtrlList[index];
    socketInfo.m_active = false;
}

IocpSocketServer::SocketCtrl& IocpSocketServer::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

void IocpSocketServer::AddSocketCtrl(int index, IocpSocket* socket) {
    m_socketCtrlList[index].m_socket = socket;
    m_socketCtrlList[index].m_active = false;
    ++m_socketCount;
}

void IocpSocketServer::RemoveSocketCtrl(int index) {
    m_socketCtrlList[index].m_socket = nullptr;
    m_socketCtrlList[index].m_active = false;
    --m_socketCount;
}

bool IocpSocketServer::AssociateWithIocp(IocpSocket* socket) {
    const SockId_t& sockId = socket->GetSockId();
    HANDLE sockHandle = reinterpret_cast<HANDLE>(sockId);
    ULONG_PTR compKey = reinterpret_cast<ULONG_PTR>(socket);
    HANDLE hIocp = ::CreateIoCompletionPort(sockHandle, m_hIocp, compKey, 0);
    if (IsNull(hIocp)) {
        return false;
    }
    return true;
}

int IocpSocketServer::GetVacantIndex() const {
    for (int index = 0; index < m_socketCapacity; ++index) {
        if (IsNull(m_socketCtrlList[index].m_socket)) {
            return index;
        }
    }

    ASSERT_TRUE(false);
    return -1;
}

IocpIoThread::IocpIoThread()
    : m_socketServer(nullptr)
    , m_stopDesired(false) {}

IocpIoThread::~IocpIoThread() {}

void IocpIoThread::Initialize(IocpSocketServer* socketServer) {
    m_socketServer = socketServer;
    m_stopDesired = false;
}

void IocpIoThread::Finalize() {
    Stop();
}

void IocpIoThread::Stop() {
    m_stopDesired = true;
    Wait();
}

void IocpIoThread::Execute() {
    IocpSocket* iocpSocket = nullptr;
    IocpOperation* iocpOperation = nullptr;
    DWORD numberOfBytes = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED overLapped = nullptr;
    HANDLE hIocp = m_socketServer->GetIocpHandle();

    HANDLE hThread = ::GetCurrentThread();
    ::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

    while (!m_stopDesired) {
        numberOfBytes = 0;
        completionKey = 0;
        overLapped = nullptr;

        BOOL ret = ::GetQueuedCompletionStatus(hIocp
            , &numberOfBytes
            , &completionKey
            , &overLapped
            , 200);

        iocpSocket = reinterpret_cast<IocpSocket*>(completionKey);
        iocpOperation = CONTAINING_RECORD(overLapped, IocpOperation, m_overlapped);

        const DWORD error = ::WSAGetLastError();
        const int result = AnalyseStatusResult(ret
            , overLapped
            , numberOfBytes
            , error);

        switch (result) {
        case RESULT_SUCCESS:
            iocpSocket->HandleSuccessOperation(iocpOperation, numberOfBytes);
            break;
        case RESULT_FAILURE:
            iocpSocket->HandleFailureOperation(iocpOperation, numberOfBytes, error);
            break;
        case RESULT_CLOSE:
            iocpSocket->HandleCloseOperation(iocpOperation, numberOfBytes);
            break;
        case RESULT_TIMEOUT:
            break;
        case RESULT_ERROR:
            break;
        default: ASSERT_TRUE(false); break;
        }
    }
}

int IocpIoThread::AnalyseStatusResult(BOOL ret
    , OVERLAPPED* overlapped
    , DWORD numberOfBytes
    , DWORD error) {
    if (TRUE == ret) {
        if (0xFFFFFFFF == numberOfBytes) {
            return RESULT_CLOSE;
        }

        return RESULT_SUCCESS;
    }

    if (!IsNull(overlapped)) {
        return RESULT_FAILURE;
    }

    if (WAIT_TIMEOUT == error) {
        return RESULT_TIMEOUT;
    }

    return RESULT_ERROR;
}

IocpMaintenanceThread::IocpMaintenanceThread()
    : m_socketServer(nullptr)
    , m_interval(0)
    , m_stopDesired(false) {}

IocpMaintenanceThread::~IocpMaintenanceThread() {}

void IocpMaintenanceThread::Init(IocpSocketServer* socketServer) {
    ASSERT_TRUE(!IsNull(socketServer));

    m_socketServer = socketServer;
    m_interval = 0;
    m_stopDesired = false;
}

void IocpMaintenanceThread::Finalize() {
    Stop();
}

void IocpMaintenanceThread::Stop() {
    m_stopDesired = true;
    Wait();
}

void IocpMaintenanceThread::Execute() {
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

void IocpMaintenanceThread::ProcessSockets() {
    const int socketCapacity = m_socketServer->GetSocketCapacity();
    for (int index = 0; index < socketCapacity; ++index) {
        IocpSocketServer::SocketCtrl& socketCtrl = m_socketServer->GetSocketCtrl(index);
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

void IocpMaintenanceThread::ProcessActiveSocket(IocpSocket* iocpSocket, int index) {
    if (iocpSocket->IsClosed()) {
        m_socketServer->MarkSocketClosed(index);
        return;
    }

    iocpSocket->Tick(m_interval);

    iocpSocket->PostInputOperation();
    iocpSocket->PostOutputOperation();
}

void IocpMaintenanceThread::ProcessClosedSocket(IocpSocket* iocpSocket, int index) {
    if (iocpSocket->IsActive()) {
        m_socketServer->MarkSocketActive(index);
        return;
    }

    iocpSocket->Tick(m_interval);

    if (!iocpSocket->IsClosing()) {
        m_socketServer->Lock();
        if (iocpSocket->IsClosing()) {
            m_socketServer->Unlock();
            return;
        }

        m_socketServer->RemoveSocketCtrl(index);
        m_socketServer->Unlock();

        iocpSocket->OnDetached();
        iocpSocket->Destory();
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
