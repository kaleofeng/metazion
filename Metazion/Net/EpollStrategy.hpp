#ifndef MZ_NET_EPOLLSTRATEGY_HPP
#define MZ_NET_EPOLLSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

class EpollStrategy {
    DISALLOW_COPY_AND_ASSIGN(EpollStrategy)

public:
    EpollStrategy() {}

    virtual ~EpollStrategy() {}

public:
    virtual void Reset() = 0;

    virtual bool IsBusy() const = 0;

    virtual void Input() = 0;

    virtual void Output() = 0;

    virtual void EnableOutput() = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLSTRATEGY_HPP
