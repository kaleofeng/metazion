#include "Metazion/Net/IocpService.hpp"

#include "Metazion/Net/Socket.hpp"

#if defined(MZ_PLATFORM_WINOWS)

DECL_NAMESPACE_MZ_NET_BEGIN

IocpService::IocpService()
    : m_socketCapacity(0)
    , m_ioThreadNumber(0)
    , m_hIocp(nullptr) {}

IocpService::~IocpService() {}

bool IocpService::Initialize(int socketCapacity, int ioThreadNumber) {
    m_socketCapacity = socketCapacity;
    m_ioThreadNumber = ioThreadNumber;

    auto hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, ioThreadNumber);
    if (hIocp == NULL) {
        return false;
    }

    m_hIocp = hIocp;
    return true;
}

void IocpService::Finalize() {
    ::CloseHandle(m_hIocp);
}

bool IocpService::Associate(Socket* socket) {
    const auto& sockId = socket->GetSockId();
    auto sockHandle = reinterpret_cast<HANDLE>(sockId);
    auto compKey = reinterpret_cast<ULONG_PTR>(socket);
    auto hIocp = ::CreateIoCompletionPort(sockHandle, m_hIocp, compKey, 0);
    return hIocp != NULL;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS
