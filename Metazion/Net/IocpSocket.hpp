#ifndef MZ_NET_IOCPSOCKET_HPP
#define MZ_NET_IOCPSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/IocpOperation.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class IocpSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(IocpSocket)

    friend class IocpIoThread;
    friend class IocpMaintenanceThread;
    friend class IocpSocketServer;

public:
    IocpSocket();

    virtual ~IocpSocket();

protected:
    virtual bool PostInputOperation() = 0;

    virtual bool PostOutputOperation() = 0;

    virtual bool HandleSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) = 0;

    virtual bool HandleFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) = 0;

    virtual bool HandleCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_NET_IOCPSOCKET_HPP
