#include "Metazion/Net/SelectService.hpp"

#include "Metazion/Net/Socket.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

SelectService::SelectService() {}

SelectService::~SelectService() {}

bool SelectService::Initialize(int socketCapacity, int ioThreadNumber) {
    m_socketCapacity = socketCapacity;
    m_ioThreadNumber = ioThreadNumber;
    return true;
}

void SelectService::Finalize() {}

bool SelectService::Associate(Socket* socket) {
    return true;
}

int SelectService::GetThreadIndex(int socketIndex) {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return socketIndex / eachCount;
}

int SelectService::GetStartIndex(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return eachCount * threadIndex;
}

int SelectService::GetSocketCount(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    const auto startIndex = eachCount * threadIndex;
    const auto restCount = m_socketCapacity - startIndex;
    return restCount < eachCount ? restCount : eachCount;
}

DECL_NAMESPACE_MZ_NET_END

#endif
