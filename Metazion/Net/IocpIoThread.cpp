#include "Metazion/Net/IocpIoThread.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

#include "Metazion/Net/NetworkService.hpp"
#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

IocpIoThread::IocpIoThread() {}

IocpIoThread::~IocpIoThread() {}

void IocpIoThread::Initialize(NetworkService* networkService, int index) {
    m_stopDesired = false;
    m_networkService = networkService;
    m_index = index;
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

    auto hIocp = m_networkService->GetSocketService().GetIocpHandle();

    auto hThread = GetCurrentThread();
    SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

    while (!m_stopDesired) {
        numberOfBytes = 0;
        completionKey = 0;
        overlapped = nullptr;

        auto ret = GetQueuedCompletionStatus(hIocp
            , &numberOfBytes
            , &completionKey
            , &overlapped
            , 20);

        socket = reinterpret_cast<Socket*>(completionKey);
        iocpOperation = CONTAINING_RECORD(overlapped, IocpOperation, m_overlapped);

        const auto error = WSAGetLastError();
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
        default: MZ_ASSERT_TRUE(false); break;
        }
    }
}

int IocpIoThread::AnalyseStatusResult(BOOL ret
    , OVERLAPPED* overlapped
    , DWORD numberOfBytes
    , DWORD error) {
    if (ret == TRUE) {
        if (numberOfBytes == 0xFFFFFFFF) {
            return RESULT_CLOSE;
        }

        return RESULT_SUCCESS;
    }

    if (!IsNull(overlapped)) {
        return RESULT_FAILURE;
    }

    if (error == WAIT_TIMEOUT) {
        return RESULT_TIMEOUT;
    }

    return RESULT_ERROR;
}

DECL_NAMESPACE_MZ_NET_END

#endif
