#ifndef _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
#define _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class EpollTransmitStrategy final : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollTransmitStrategy)

public:
    EpollTransmitStrategy(TransmitSocket& transmitSocket);

    ~EpollTransmitStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Tick(int interval) override final;

    void Start() override final;

    bool IsBusy() const override final;

    void DoInput() override final;

    void DoOutput() override final;

    void EnableOutput() override final;

private:
    TransmitSocket& m_transmitSocket;
    volatile bool m_canOutput;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_EPOLL_MODEL

#endif // _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
