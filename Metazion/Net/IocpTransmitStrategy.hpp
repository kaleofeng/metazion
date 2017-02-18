#ifndef _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_
#define _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class IocpTransmitStrategy final : public IocpStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(IocpTransmitStrategy)

private:
    TransmitSocket& m_transmitSocket;
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;

public:
    IocpTransmitStrategy(TransmitSocket& transmitSocket);

    ~IocpTransmitStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    void Launch() override final;

    bool IsBusy() const override final;

    bool PostInput() override final;

    bool PostOutput() override final;

    bool HandleSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber) override final;

    bool HandleFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) override final;

    bool HandleClose(const IocpOperation* iocpOperation
        , DWORD byteNumber) override final;

private:
    bool PostRecv();

    bool PostSend();

    bool HandleRecvSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleSendSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleRecvFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool HandleSendFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool HandleRecvClose(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool HandleSendClose(const IocpOperation* iocpOperation
        , DWORD byteNumber);
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_
