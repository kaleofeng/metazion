#ifndef _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_
#define _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class SelectTransmitStrategy final : public SelectStrategy {
    DISALLOW_COPY_AND_ASSIGN(SelectTransmitStrategy)

public:
    SelectTransmitStrategy(TransmitSocket& transmitSocket);

    ~SelectTransmitStrategy();

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

#endif // NETWORK_USE_SELECT_MODEL

#endif // _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_
