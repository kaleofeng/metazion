#ifndef _MZ_NET_EPOLLSTRATEGY_HPP_
#define _MZ_NET_EPOLLSTRATEGY_HPP_

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

    virtual void Prepare() = 0;

    virtual void Tick(int interval) = 0;

    virtual void Start() = 0;

    virtual bool IsBusy() const = 0;

    virtual void DoInput() = 0;

    virtual void DoOutput() = 0;

    virtual void EnableOutput() = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_NET_EPOLLSTRATEGY_HPP_
