#ifndef _MZ_NET_SELECTSTRATEGY_HPP_
#define _MZ_NET_SELECTSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

class SelectStrategy {
    DISALLOW_COPY_AND_ASSIGN(SelectStrategy)

public:
    SelectStrategy() {}

    virtual ~SelectStrategy() {}

public:
    virtual void Reset() = 0;

    virtual void Prepare() = 0;

    virtual void Tick(int interval) = 0;

    virtual void Start() = 0;

    virtual bool IsBusy() const = 0;

    virtual void DoInput() = 0;

    virtual void DoOutput() = 0;

    virtual void EnableOutput() = 0;

    virtual bool ShouldCareAboutOutput() const = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_SELECT_MODEL

#endif // _MZ_NET_SELECTSTRATEGY_HPP_
