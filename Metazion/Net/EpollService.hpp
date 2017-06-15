#ifndef _MZ_NET_EPOLLSERVICE_HPP_
#define _MZ_NET_EPOLLSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/SocketDefine.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;

class EpollService {
    MZ_DISALLOW_COPY_AND_ASSIGN(EpollService)

private:
    int m_socketCapacity{ 0 };
    int m_ioThreadNumber{ 0 };
    int* m_epollfdList{ 0 };
    epoll_event* m_epollEventList{ nullptr };

public:
    EpollService();

    ~EpollService();

    bool Initialize(int socketCapacity, int ioThreadNumber);

    void Finalize();

    bool Associate(Socket* socket);

    int GetThreadIndex(int socketIndex);

    int GetStartIndex(int threadIndex) const;

    int GetSocketCount(int threadIndex) const;

    int GetEpollfd(int threadIndex) const;

    epoll_event& GetEpollEvent(int index);
};

inline int EpollService::GetEpollfd(int threadIndex) const {
    return m_epollfdList[threadIndex];
}

inline epoll_event& EpollService::GetEpollEvent(int index) {
    return m_epollEventList[index];
}

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_EPOLLSERVICE_HPP_
