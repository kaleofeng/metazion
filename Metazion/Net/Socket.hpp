#ifndef _MZ_NET_SOCKET_HPP_
#define _MZ_NET_SOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <atomic>
#include <functional>

#include "Metazion/Net/AlternativeStrategy.hpp"
#include "Metazion/Net/Host.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class Socket {
    MZ_DISALLOW_COPY_AND_ASSIGN(Socket)

    using DestoryCallback_t = std::function<void(Socket* socket)>;

public:
    Socket();

    virtual ~Socket();

public:
    virtual void Reset();

    virtual void Prepare();

    virtual void Tick(int interval) = 0;

    virtual void Dispatch() = 0;

    virtual int GetType() const = 0;

    virtual IoStrategy& GetIoStrategy() = 0;

    virtual bool IsAlive() const = 0;

    virtual void OnAttached() = 0;

    virtual void OnDetached() = 0;

    virtual void OnStart() = 0;

    virtual void OnStop() = 0;

    virtual void OnError(int error) = 0;

public:
    void Retain();

    void Release();

    void Close();

    void Destory();

    void Start();
    
    void Stop();

    bool IsValid() const;

    bool IsActive() const;

    bool IsWorking() const;

    bool IsWannaClose() const;

    const SockId_t& GetSockId() const;

    void AttachSockId(const SockId_t& sockId);

    void DetachSockId();

    int GetIndex() const;

    void SetIndex(int index);

    NetworkService* GetNetworkService();

    void SetNetworkService(NetworkService* networkService);

    void SetDestroyCallback(DestoryCallback_t callback);

protected:
    std::atomic<int> m_reference;
    volatile bool m_working;
    volatile bool m_wannaClose;
    SockId_t m_sockId;
    int m_index;
    NetworkService* m_networkService;
    DestoryCallback_t m_destroyCallback;
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

inline NetworkService* Socket::GetNetworkService() {
    return m_networkService;
}

inline void Socket::SetNetworkService(NetworkService* networkService) {
    m_networkService = networkService;
}

inline void Socket::SetDestroyCallback(DestoryCallback_t callback) {
    m_destroyCallback = callback;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKET_HPP_
