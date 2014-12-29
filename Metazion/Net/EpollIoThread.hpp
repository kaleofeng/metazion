#ifndef _MZ_NET_EPOLLIOTHREAD_HPP_
#define _MZ_NET_EPOLLIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class EpollIoThread : public NS_SHARE::Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(EpollIoThread)

public:
    EpollIoThread();

    ~EpollIoThread();

public:
    void Initialize(NetworkService* networkService, int index);

    void Finalize();

protected:
    void Execute() override final;

private:
    void ProcessEvents();

private:
    volatile bool m_stopDesired;

    NetworkService* m_networkService;
    int m_index;
    int m_socketCount;
    SocketCtrl* m_socketCtrlList;
    int m_epollfd;
    epoll_event* m_eventList;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_EPOLL_MODEL

#endif // _MZ_NET_EPOLLIOTHREAD_HPP_
