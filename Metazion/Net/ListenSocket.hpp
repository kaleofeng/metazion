#ifndef _MZ_NET_LISTENSOCKET_HPP_
#define _MZ_NET_LISTENSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket : public Socket {
    MZ_DISALLOW_COPY_AND_ASSIGN(ListenSocket)

    friend class ListenStrategy;

protected:
    ListenStrategy m_listenStrategy;
    Host m_localHost;

    int64_t m_watchedTime{ 0 };
    int64_t m_unwatchedTime{ 0 };
    int64_t m_firstAcceptTime{ 0 };
    int64_t m_lastAcceptTime{ 0 };
    int64_t m_acceptedNumber{ 0 };

public:
    ListenSocket();

    virtual ~ListenSocket();

    void Reset() override;

    void Prepare() override;

    bool IsAlive() const override final;

    bool KeepEnough() const override final;

    IoStrategy& TheIoStrategy() override final;

    void OnAttached() override final;

    void OnDetached() override final;

    void OnStart() override final;

    void OnStop() override final;

    void OnError(int error) override final;

    void SetLocalHost(const Host& host);

    bool Listen(int backlog);

protected:
    virtual void OnWatched();

    virtual void OnUnwatched();

    virtual bool OnAccepted(const SockId_t& sockId, const Host& host);
};

inline IoStrategy& ListenSocket::TheIoStrategy() {
    return m_listenStrategy;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_LISTENSOCKET_HPP_
