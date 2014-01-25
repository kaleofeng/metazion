#ifndef MZ_NET_IOCPTRANSMITSTRATEGY_HPP
#define MZ_NET_IOCPTRANSMITSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class IocpTransmitStrategy final : public IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpTransmitStrategy)

public:
    IocpTransmitStrategy(TransmitSocket& transmitSocket);

    virtual ~IocpTransmitStrategy();

public:
    void Reset() override final;

    bool IsBusy() const override final;

    bool PostInputOperation() override final;

    bool PostOutputOperation() override final;

    bool HandleSuccessOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) override final;

    bool HandleFailureOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) override final;

    bool HandleCloseOperation(const IocpOperation* iocpOperation
        , DWORD byteNumber) override final;

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

#endif // MZ_NET_IOCPTRANSMITSTRATEGY_HPP
