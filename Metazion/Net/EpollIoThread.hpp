#ifndef _MZ_NET_EPOLLIOTHREAD_HPP_
#define _MZ_NET_EPOLLIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class EpollIoThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(EpollIoThread)

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

    void ProcessSockets();

private:
    NetworkService* m_networkService;
    int m_index;
    int m_epollfd;
    epoll_event* m_eventList;
    int m_socketCount;
    SocketCtrl* m_socketCtrlList;
    volatile bool m_stopDesired;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_NET_EPOLLIOTHREAD_HPP_
