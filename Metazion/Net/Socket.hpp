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

    friend class NetworkService;
    friend class IoThread;
    friend class MaintenanceThread;

    using DestoryCallback_t = std::function<void(Socket* socket)>;

protected:
    std::atomic<int> m_reference = { 0 };
    std::atomic<bool> m_working = { false };
    std::atomic<bool> m_gonnaClose = { false };
    std::atomic<bool> m_wannaClose = { false };
    SockId_t m_sockId{ INVALID_SOCKID };
    int m_index{ -1 };
    NetworkService* m_networkService{ nullptr };
    DestoryCallback_t m_destroyCallback = [](Socket* socket) { delete socket; };

public:
    Socket();

    virtual ~Socket();

    virtual void Reset();

    virtual void Prepare();

    virtual void Tick(int interval) = 0;

    virtual void Dispatch() = 0;

    void Retain();

    void Release();

    void Disconnect();

    void Destory();

    bool IsValid() const;

    bool IsActive() const;

    bool IsWorking() const;

    bool IsWannaClose() const;

    const SockId_t& GetSockId() const;

    void AttachSockId(const SockId_t& sockId);

    void DetachSockId();

    int GetIndex() const;

    NetworkService* GetNetworkService();

    void SetDestroyCallback(DestoryCallback_t callback);

protected:
    virtual int GetType() const = 0;

    virtual IoStrategy& GetIoStrategy() = 0;

    virtual bool IsAlive() const = 0;

    virtual void OnAttached() = 0;

    virtual void OnDetached() = 0;

    virtual void OnStart() = 0;

    virtual void OnStop() = 0;

    virtual void OnError(int error) = 0;

    void Close();

    void Start();

    void Stop();

    bool IsGonnaClose() const;

    void SetIndex(int index);

    void SetNetworkService(NetworkService* networkService);
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

inline bool Socket::IsGonnaClose() const {
    return m_gonnaClose;
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
