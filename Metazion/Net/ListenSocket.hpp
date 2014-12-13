#ifndef _MZ_NET_LISTENSOCKET_HPP_
#define _MZ_NET_LISTENSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/RingBuffer.hpp>

#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket : public Socket {
    MZ_DISALLOW_COPY_AND_ASSIGN(ListenSocket)

    friend class ListenStrategy;

public:
    ListenSocket();

    virtual ~ListenSocket();

public:
    void Reset() override;

    void Prepare() override;

    void Tick(int interval) override;

    IoStrategy& GetIoStrategy() override final;

    bool IsAlive() const override final;

    void OnAttached() override final;

    void OnDetached() override final;

    void OnStart() override final;

    void OnClose() override final;

    bool OnError(int error) override final;

    void SetLocalHost(const Host& host);

    bool Listen(int backlog);

protected:
    virtual void OnWatched() = 0;

    virtual void OnUnwatched() = 0;

    virtual bool OnAccepted(const SockId_t& sockId) = 0;

protected:
    Host m_localHost;

private:
    ListenStrategy m_listenStrategy;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_LISTENSOCKET_HPP_
