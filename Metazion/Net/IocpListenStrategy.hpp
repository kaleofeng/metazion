#ifndef _MZ_NET_IOCPLISTENSTRATEGY_HPP_
#define _MZ_NET_IOCPLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/RingBuffer.hpp>

#include "Metazion/Net/IocpStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class IocpListenStrategy final : public IocpStrategy {
    DISALLOW_COPY_AND_ASSIGN(IocpListenStrategy)

public:
    IocpListenStrategy(ListenSocket& listenSocket);

    ~IocpListenStrategy();

public:
    void Reset() override;

    void Prepare() override;

    void Tick(int interval) override;
    
    void Start() override;

    bool IsBusy() const override;

    bool PostInput() override;

    bool PostOutput() override;

    bool HandleSuccess(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

    bool HandleFailure(const IocpOperation* iocpOperation
        , DWORD byteNumber, int error) override;

    bool HandleClose(const IocpOperation* iocpOperation
        , DWORD byteNumber) override;

private:
    bool PostAccept();

private:
    ListenSocket& m_listenSocket;
    AcceptOperation m_acceptOperation;
    char m_acceptBuffer[256];
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_IOCP_MODEL

#endif // _MZ_NET_IOCPLISTENSTRATEGY_HPP_
