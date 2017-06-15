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
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;
    std::atomic<bool> m_canInput{ false };
    std::atomic<bool> m_canOutput{ false };

public:
    EpollTransmitStrategy(TransmitSocket& transmitSocket);

    ~EpollTransmitStrategy();

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

#endif // _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
