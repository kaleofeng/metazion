#ifndef MZ_NET_EPOLLTRANSMITSTRATEGY_HPP
#define MZ_NET_EPOLLTRANSMITSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/EpollStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class EpollTransmitStrategy : public EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollTransmitStrategy)

public:
    EpollTransmitStrategy(TransmitSocket& transmitSocket);

    virtual ~EpollTransmitStrategy();

public: // @Override
    void Reset();

    bool IsBusy() const;

    void Input();

    void Output();

    void EnableOutput();

private:
    TransmitSocket& m_transmitSocket;
    volatile bool m_canOutput;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLTRANSMITSTRATEGY_HPP
