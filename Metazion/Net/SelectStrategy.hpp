#ifndef _MZ_NET_SELECTSTRATEGY_HPP_
#define _MZ_NET_SELECTSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectOperation.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SelectStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectStrategy)

public:
    SelectStrategy() {}

    virtual ~SelectStrategy() {}

public:
    virtual void Reset() = 0;

    virtual void Prepare() = 0;

    virtual void Start() = 0;

    virtual void Launch() = 0;

    virtual bool IsBusy() const = 0;

    virtual void PostInput() = 0;

    virtual void PostOutput() = 0;

    virtual void EnableInput() = 0;

    virtual void EnableOutput() = 0;

    virtual bool ShouldCareAboutOutput() const = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTSTRATEGY_HPP_
