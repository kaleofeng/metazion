#ifndef _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
#define _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class EpollTransmitStrategy final : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollTransmitStrategy)

public:
    EpollTransmitStrategy(TransmitSocket& transmitSocket);

    ~EpollTransmitStrategy();

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
    TransmitSocket& m_transmitSocket;
    volatile bool m_canOutput;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_NET_EPOLLTRANSMITSTRATEGY_HPP_
