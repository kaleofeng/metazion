#ifndef MZ_NET_SOCKET_HPP
#define MZ_NET_SOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Sync/AutoGuard.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>
#include "Metazion/Net/SocketDefine.hpp"
#include "Metazion/Net/Host.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketServer;

class Socket {
    DISALLOW_COPY_AND_ASSIGN(Socket)

public:
    Socket();

    virtual ~Socket();

public:
    virtual int GetType() const = 0;

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

    virtual bool IsActive();
    
    virtual bool IsClosed();

    virtual bool IsClosing();

public:
    void Rework();
    
    void Close();

    bool IsValid() const;

    bool IsReady() const;

    int GetRefCount() const;

    void GrabRef();

    void ReleaseRef();

    bool IsIoAvailable() const;

    const SockId_t& GetSockId() const;

    void OpenSockId(const SockId_t& sockId);

    void CloseSockId();

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
    volatile int m_refCount;
    volatile bool m_ioAvailable;
    SockId_t m_sockId;
    int m_index;
    SocketServer* m_socketServer;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_SOCKET_HPP
