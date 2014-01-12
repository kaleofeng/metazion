#ifndef MZ_NET_EPOLLLISTENSTRATEGY_HPP
#define MZ_NET_EPOLLLISTENSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class EpollListenStrategy : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollListenStrategy)

public:
    EpollListenStrategy(ListenSocket& m_listenSocket);

    virtual ~EpollListenStrategy();

public: // @Override
    void Reset();

    bool IsBusy() const;

    void Input();

    void Output();

    void EnableOutput();

private:
    ListenSocket& m_listenSocket;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLLISTENSTRATEGY_HPP
