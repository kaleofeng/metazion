#ifndef _MZ_NET_SOCKET_HPP_
#define _MZ_NET_SOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <mutex>

#include "Metazion/Net/AlternativeStrategy.hpp"
#include "Metazion/Net/Host.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketServer;

class Socket {
    DISALLOW_COPY_AND_ASSIGN(Socket)

    using Lock_t = std::mutex;
    using LockGuard_t = std::lock_guard<Lock_t>;

public:
    Socket();

    virtual ~Socket();

public:
    virtual int GetType() const = 0;

    virtual IoStrategy& GetIoStrategy() = 0;

    virtual void Reset();

    virtual void Prepare();

    virtual void Destory();

    virtual void Tick(int interval);

    virtual void Dispatch();

    virtual void OnAttached();

    virtual void OnDetached();

    virtual void OnStart();

    virtual void OnClose();

    virtual void OnStarted();

    virtual void OnClosed();

    virtual bool OnError(int error);

    virtual bool IsAlive() const;

public:
    void Retain();

    void Release();

    void Start();
    
    void Close();

    void DoClose();

    bool IsValid() const;

    bool IsActive() const;

    bool IsWorking() const;

    bool IsWannaClose() const;

    const SockId_t& GetSockId() const;

    void AttachSockId(const SockId_t& sockId);

    void DetachSockId();

    int GetIndex() const;

    void SetIndex(int index);

    SocketServer* GetSocketServer();

    void SetSocketServer(SocketServer* socketServer);

protected:
    Lock_t m_lock;
    volatile int m_reference;
    volatile bool m_working;
    volatile bool m_wannaClose;
    SockId_t m_sockId;
    int m_index;
    SocketServer* m_socketServer;
};

inline bool Socket::IsValid() const {
    return m_reference > 0;
}

inline bool Socket::IsActive() const {
    return m_sockId != INVALID_SOCKID;
}

inline bool Socket::IsWorking() const {
    return m_working;
}

inline bool Socket::IsWannaClose() const {
    return m_wannaClose;
}

inline const SockId_t& Socket::GetSockId() const {
    return m_sockId;
}

inline void Socket::AttachSockId(const SockId_t& sockId) {
    m_sockId = sockId;
}

inline void Socket::DetachSockId() {
    if (m_sockId != INVALID_SOCKID) {
        DestroySockId(m_sockId);
        m_sockId = INVALID_SOCKID;
    }
}

inline int Socket::GetIndex() const {
    return m_index;
}

inline void Socket::SetIndex(int index) {
    m_index = index;
}

inline SocketServer* Socket::GetSocketServer() {
    return m_socketServer;
}

inline void Socket::SetSocketServer(SocketServer* socketServer) {
    m_socketServer = socketServer;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKET_HPP_
