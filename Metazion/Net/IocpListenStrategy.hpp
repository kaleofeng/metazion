#ifndef MZ_NET_IOCPLISTENSTRATEGY_HPP
#define MZ_NET_IOCPLISTENSTRATEGY_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/RingBuffer.hpp>
#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class IocpListenStrategy : public IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpListenStrategy)

public:
    IocpListenStrategy(ListenSocket& listenSocket);

    virtual ~IocpListenStrategy();

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
    bool _PostAcceptOperation();

private:
    ListenSocket& m_listenSocket;
    AcceptOperation m_acceptOperation;
    char m_acceptBuffer[256];
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_NET_IOCPLISTENSTRATEGY_HPP
