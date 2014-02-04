#ifndef _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_
#define _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class IocpTransmitStrategy final : public IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpTransmitStrategy)

public:
    IocpTransmitStrategy(TransmitSocket& transmitSocket);

    ~IocpTransmitStrategy();

public:
    void Reset() override;

    bool IsBusy() const override;

    bool PostInputOperation() override;

    bool PostOutputOperation() override;

    bool HandleSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

    bool HandleFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) override;

    bool HandleCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

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

private:
    TransmitSocket& m_transmitSocket;
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_
