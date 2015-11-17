#ifndef _MZ_NET_EPOLLIOTHREAD_HPP_
#define _MZ_NET_EPOLLIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include <atomic>

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class EpollIoThread : public NS_SHARE::Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(EpollIoThread)

private:
    std::atomic<bool> m_stopDesired = { false };

    NetworkService* m_networkService = nullptr;
    int m_index = 0;
    int m_socketCount = 0;
    SocketCtrl* m_socketCtrlList = nullptr;
    int m_epollfd = 0;
    epoll_event* m_eventList = nullptr;

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
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_EPOLLIOTHREAD_HPP_
