#ifndef _MZ_NET_EPOLLLISTENSTRATEGY_HPP_
#define _MZ_NET_EPOLLLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class EpollListenStrategy final : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollListenStrategy)

public:
    EpollListenStrategy(ListenSocket& m_listenSocket);

    ~EpollListenStrategy();

public:
    void Reset() override;

    void Prepare() override;

    void Tick(int interval) override;

    void Start() override;

    bool IsBusy() const override;

    void DoInput() override;

    void DoOutput() override;

    void EnableOutput() override;

private:
    ListenSocket& m_listenSocket;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_NET_EPOLLLISTENSTRATEGY_HPP_
