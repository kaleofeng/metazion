#include "Metazion/Net/EpollService.hpp"

#include <Metazion/Share/Utility/Random.hpp>

#include "Metazion/Net/Socket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

static NS_SHARE::Random g_random;

EpollService::EpollService()
    : m_socketCapacity(0)
    , m_ioThreadNumber(0)
    , m_epollfdList(nullptr)
    , m_epollEventList(nullptr) {}

EpollService::~EpollService() {}

bool EpollService::Initialize(int socketCapacity, int ioThreadNumber) {
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

    m_epollEventList = new epoll_event[m_socketCapacity];
    ::memset(m_epollEventList, 0, sizeof(epoll_event) * m_socketCapacity);
}

void EpollService::Finalize() {
    SafeDeleteArray(m_epollEventList);

    for (auto index = 0; index < m_ioThreadNumber; ++index) {
        auto& epollfd = m_epollfdList[index];
        if (epollfd >= 0) {
            ::close(epollfd);
        }
    }
    SafeDeleteArray(m_epollfdList);
}

bool EpollService::Associate(Socket* socket) {
    const auto& sockId = socket->GetSockId();
    const auto socketIndex = socket->GetIndex();
    const auto threadIndex = GetThreadIndex(socketIndex);

    epoll_event event;
    event.data.ptr = socket;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    const auto ret = ::epoll_ctl(m_epollfdList[threadIndex], EPOLL_CTL_ADD
        , sockId, &event);
    return ret == 0;
}

int EpollService::GetThreadIndex(int socketIndex) {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return socketIndex / eachCount;
}

int EpollService::GetStartIndex(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    return eachCount * threadIndex;
}

int EpollService::GetSocketCount(int threadIndex) const {
    const auto eachCount = (m_socketCapacity + m_ioThreadNumber - 1) / m_ioThreadNumber;
    const auto startIndex = eachCount * threadIndex;
    const auto restCount = m_socketCapacity - startIndex;
    return restCount < eachCount ? restCount : eachCount;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
