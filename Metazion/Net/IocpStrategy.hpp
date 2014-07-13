#ifndef _MZ_NET_IOCPSOCKET_HPP_
#define _MZ_NET_IOCPSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/IocpOperation.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpStrategy)

public:
    IocpStrategy() {}

    virtual ~IocpStrategy() {}

public:
    virtual void Reset() = 0;

    virtual void Prepare() = 0;

    virtual void Start() = 0;

    virtual bool IsBusy() const = 0;

    virtual bool PostInput() = 0;

    virtual bool PostOutput() = 0;

    virtual bool HandleSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber) = 0;

    virtual bool HandleFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) = 0;

    virtual bool HandleClose(const IocpOperation* iocpOperation
        , DWORD byteNumber) = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPSOCKET_HPP_
