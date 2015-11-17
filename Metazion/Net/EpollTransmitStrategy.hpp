#ifndef _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
#define _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include <atomic>

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class EpollTransmitStrategy final : public EpollStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(EpollTransmitStrategy)

private:
    TransmitSocket& m_transmitSocket;
    std::atomic<bool> m_canOutput = { false };

public:
    EpollTransmitStrategy(TransmitSocket& transmitSocket);

    ~EpollTransmitStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    bool IsBusy() const override final;

    void PostInput() override final;

    void PostOutput() override final;

    void EnableOutput() override final;
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
