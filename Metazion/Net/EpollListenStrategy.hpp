#ifndef _MZ_NET_EPOLLLISTENSTRATEGY_HPP_
#define _MZ_NET_EPOLLLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class EpollListenStrategy final : public EpollStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(EpollListenStrategy)

private:
    ListenSocket& m_listenSocket;
    std::atomic<bool> m_canInput{ false };
    std::atomic<bool> m_canOutput{ false };

public:
    EpollListenStrategy(ListenSocket& m_listenSocket);

    ~EpollListenStrategy();

    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    void Launch() override final;

    bool IsBusy() const override final;

    void PostInput() override final;

    void PostOutput() override final;

    void EnableInput() override final;

    void EnableOutput() override final;
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_EPOLLLISTENSTRATEGY_HPP_
