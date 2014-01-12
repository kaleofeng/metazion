#ifndef MZ_NET_SOCKET_HPP
#define MZ_NET_SOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Sync/AutoGuard.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>
#include "Metazion/Net/Host.hpp"
#include "Metazion/Net/IoStrategy.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketServer;

class Socket {
    DISALLOW_COPY_AND_ASSIGN(Socket)

public:
    Socket();

    virtual ~Socket();

public:
    virtual int GetType() const = 0;

    virtual IoStrategy& GetIoStrategy() = 0;

    virtual void Reset();

    virtual void Destory();

    virtual void Tick(int interval);

    virtual void OnAttached();

    virtual void OnDetached();

    virtual void OnStarted();

    virtual void OnClosed();

    virtual bool OnAccepted(const SockId_t& sockId);

    virtual int OnSendData(const void* data, int length);

    virtual int OnRecvData(const void* data, int length);

    virtual bool OnError(int error);

    virtual bool IsActive() const;
    
    virtual bool IsClosed() const;

    virtual bool IsAlive() const;

public:
    void Start();
    
    void Close();

    bool IsValid() const {
        return m_refCount > 0;
    }

    bool IsReady() const {
        return INVALID_SOCKID != m_sockId;
    }

    bool IsWorking() const {
        return m_working;
    }

    int GetRefCount() const {
        return m_refCount;
    }

    void GrabRef();

    void ReleaseRef();

    const SockId_t& GetSockId() const {
        return m_sockId;
    }

    void AttachSockId(const SockId_t& sockId);

    void DetachSockId();

    int GetIndex() const {
        return m_index;
    }

    void SetIndex(int index) {
        m_index = index;
    }

    SocketServer* GetSocketServer() {
        return m_socketServer;
    }

    void SetSocketServer(SocketServer* socketServer) {
        m_socketServer = socketServer;
    }

protected:
    NS_SHARE::MutexLock m_lock;
    volatile bool m_working;
    volatile int m_refCount;
    SockId_t m_sockId;
    int m_index;
    SocketServer* m_socketServer;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_SOCKET_HPP
