#ifndef MZ_NET_EPOLLLISTENSTRATEGY_HPP
#define MZ_NET_EPOLLLISTENSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class EpollListenStrategy final : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollListenStrategy)

public:
    EpollListenStrategy(ListenSocket& m_listenSocket);

    virtual ~EpollListenStrategy();

public:
    void Reset() override final;

    bool IsBusy() const override final;

    void Input() override final;

    void Output() override final;

    void EnableOutput() override final;

private:
    ListenSocket& m_listenSocket;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLLISTENSTRATEGY_HPP
