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
    std::atomic<int> m_reference{ 0 };
    SockId_t m_sockId{ INVALID_SOCKID };
    int m_index{ -1 };
    NetworkService* m_networkService{ nullptr };
    int m_keepInterval{ 0 };
    DestoryCallback_t m_destroyCallback{ nullptr };

    int64_t m_now{ 0 };
    std::atomic<bool> m_working{ false };
    std::atomic<bool> m_gonnaClose{ false };
    std::atomic<int> m_delayTime{ 0 };
    std::atomic<bool> m_wannaClose{ false };

public:
    Socket();

    virtual ~Socket();

    virtual void Reset();

    virtual void Prepare();

    virtual void Tick(int64_t now, int interval);

    virtual void Dispatch();

    void Retain();

    void Release();

    void Destory();

    const SockId_t& GetSockId() const;

    void AttachSockId(const SockId_t& sockId);

    void DetachSockId();

    int GetIndex() const;

    NetworkService* GetNetworkService();

    void SetKeepInterval(int interval);

    void SetDestroyCallback(DestoryCallback_t callback);

    void Disconnect(int delayTime = 0);

    bool IsWorking() const;

    bool IsGonnaClose() const;

    bool IsWannaClose() const;

    bool IsValid() const;

    bool IsActive() const;

protected:
    virtual int GetType() const = 0;

    virtual bool IsAlive() const = 0;

    virtual bool KeepEnough() const = 0;

    virtual IoStrategy& TheIoStrategy() = 0;

    virtual void OnAttached() = 0;

    virtual void OnDetached() = 0;

    virtual void OnStart() = 0;

    virtual void OnStop() = 0;

    virtual void OnError(int error) = 0;

    void Shutdown();

    void Close();

    void Start();

    void Stop();

private:
    void SetIndex(int index);

    void SetNetworkService(NetworkService* networkService);
};

inline const SockId_t& Socket::GetSockId() const {
    return m_sockId;
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

inline void Socket::SetKeepInterval(int interval) {
    m_keepInterval = interval;
}

inline void Socket::SetDestroyCallback(DestoryCallback_t callback) {
    m_destroyCallback = callback;
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

inline bool Socket::IsValid() const {
    return m_reference > 0;
}

inline bool Socket::IsActive() const {
    return m_sockId != INVALID_SOCKID;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKET_HPP_
