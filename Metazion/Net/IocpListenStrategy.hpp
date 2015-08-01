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
    MZ_DISALLOW_COPY_AND_ASSIGN(IocpListenStrategy)

public:
    IocpListenStrategy(ListenSocket& listenSocket);

    ~IocpListenStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

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
    bool PostAccept();

private:
    ListenSocket& m_listenSocket;
    AcceptOperation m_acceptOperation;
    char m_acceptBuffer[256] = { 0 };
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_IOCPLISTENSTRATEGY_HPP_
