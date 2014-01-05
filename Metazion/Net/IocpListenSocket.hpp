#ifndef MZ_NET_IOCPLISTENSOCKET_HPP
#define MZ_NET_IOCPLISTENSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/RingBuffer.hpp>
#include "Metazion/Net/IocpSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class IocpListenSocket : public IocpSocket {
    DISALLOW_COPY_AND_ASSIGN(IocpListenSocket)

public:
    IocpListenSocket();

    virtual ~IocpListenSocket();

public: // @Override
    void Reset();

    void OnStarted();

    bool IsClosing();

public:
    bool Listen(const char* ip, int port);

private: // @Override
    bool PostInputOperation();

    bool PostOutputOperation();

    bool HandleSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool HandleCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

private:
    bool _PostAcceptOperation();

protected:
    Host m_localHost;

private:
    AcceptOperation m_acceptOperation;
    char m_acceptBuffer[256];
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_NET_IOCPLISTENSOCKET_HPP
