#include "Metazion/Net/IocpIoThread.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

IocpIoThread::IocpIoThread()
    : m_socketServer(nullptr)
    , m_index(-1)
    , m_stopDesired(false) {}

IocpIoThread::~IocpIoThread() {}

void IocpIoThread::Initialize(SocketServer* socketServer, int index) {
    m_socketServer = socketServer;
    m_index = index;
    m_stopDesired = false;
}

void IocpIoThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void IocpIoThread::Execute() {
    Socket* socket = nullptr;
    IocpOperation* iocpOperation = nullptr;
    DWORD numberOfBytes = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED overlapped = nullptr;

    auto hIocp = m_socketServer->GetSocketService().GetIocpHandle();

    auto hThread = ::GetCurrentThread();
    ::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

    while (!m_stopDesired) {
        numberOfBytes = 0;
        completionKey = 0;
        overlapped = nullptr;

        auto ret = ::GetQueuedCompletionStatus(hIocp
            , &numberOfBytes
            , &completionKey
            , &overlapped
            , 200);

        socket = reinterpret_cast<Socket*>(completionKey);
        iocpOperation = CONTAINING_RECORD(overlapped, IocpOperation, m_overlapped);

        const auto error = ::WSAGetLastError();
        const auto result = AnalyseStatusResult(ret
            , overlapped
            , numberOfBytes
            , error);

        switch (result) {
        case RESULT_SUCCESS:
            socket->GetIoStrategy().HandleSuccess(iocpOperation, numberOfBytes);
            break;
        case RESULT_FAILURE:
            socket->GetIoStrategy().HandleFailure(iocpOperation, numberOfBytes, error);
            break;
        case RESULT_CLOSE:
            socket->GetIoStrategy().HandleClose(iocpOperation, numberOfBytes);
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

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
