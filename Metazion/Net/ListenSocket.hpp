#ifndef _MZ_NET_LISTENSOCKET_HPP_
#define _MZ_NET_LISTENSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/PieceBuffer.hpp>
#include <Metazion/Share/Memory/RingBuffer.hpp>

#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(ListenSocket)

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

    virtual void OnWatched() = 0;

    virtual void OnUnwatched() = 0;

    virtual bool OnAccepted(const SockId_t& sockId) = 0;

    void SetLocalHost(const char* ip, int port);

    bool Listen(int backlog);

protected:
    Host m_localHost;

private:
    ListenStrategy m_listenStrategy;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_LISTENSOCKET_HPP_
