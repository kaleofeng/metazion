#ifndef _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_
#define _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class SelectTransmitStrategy final : public SelectStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectTransmitStrategy)

public:
    SelectTransmitStrategy(TransmitSocket& transmitSocket);

    ~SelectTransmitStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    bool IsBusy() const override final;

    void PostInput() override final;

    void PostOutput() override final;

    void EnableOutput() override final;

    bool ShouldCareAboutOutput() const override final;

private:
    TransmitSocket& m_transmitSocket;
    volatile bool m_canOutput;
};

inline bool SelectTransmitStrategy::ShouldCareAboutOutput() const {
    return !m_canOutput;
}

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTTRANSMITSTRATEGY_HPP_
