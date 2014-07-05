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

    void OnStarted() override;

    bool IsBusy() const override;

    bool Input() override;

    bool Output() override;

    bool OnSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

    bool OnFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) override;

    bool OnClose(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

private:
    bool PostInput();

    bool PostOutput();

    bool OnInputSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool OnOutputSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool OnInputFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool OnOutputFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error);

    bool OnInputClose(const IocpOperation* iocpOperation
        , DWORD byteNumber);

    bool OnOutputClose(const IocpOperation* iocpOperation
        , DWORD byteNumber);

private:
    TransmitSocket& m_transmitSocket;
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPTRANSMITSTRATEGY_HPP_
