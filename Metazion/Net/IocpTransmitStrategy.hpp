#ifndef MZ_NET_IOCPTRANSMITSTRATEGY_HPP
#define MZ_NET_IOCPTRANSMITSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class IocpTransmitStrategy : public IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpTransmitStrategy)

public:
    IocpTransmitStrategy(TransmitSocket& transmitSocket);

    virtual ~IocpTransmitStrategy();

public: // @Override
    void Reset();

    bool IsBusy() const;

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

private:
    TransmitSocket& m_transmitSocket;
    SendOperation m_sendOperation;
    RecvOperation m_recvOperation;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_NET_IOCPTRANSMITSTRATEGY_HPP
