#ifndef MZ_NET_IOCPTRANSMITSOCKET_HPP
#define MZ_NET_IOCPTRANSMITSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/IocpSocket.hpp"
#include "Metazion/Net/TcpSocketBuffer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class IocpTransmitSocket : public IocpSocket {
    DISALLOW_COPY_AND_ASSIGN(IocpTransmitSocket)

public:
    IocpTransmitSocket();

    virtual ~IocpTransmitSocket();

public: // @Override
    void Reset();

    void OnStarted();

    bool IsClosing();

public:
    int Send(const void* data, int length);

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
    bool _PostInputOperation();

    bool _PostOutputOperation();

    bool HandleInputSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleOutputSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleInputFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool HandleOutputFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool HandleInputCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleOutputCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber);

protected:
    TcpSocketBuffer m_socketBuffer;

private:
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_NET_IOCPTRANSMITSOCKET_HPP
