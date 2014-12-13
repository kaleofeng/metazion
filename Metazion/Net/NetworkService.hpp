#ifndef _MZ_NET_NETWORKSERVICE_HPP_
#define _MZ_NET_NETWORKSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <mutex>

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/AlternativeService.hpp"
#include "Metazion/Net/AlternativeThread.hpp"
#include "Metazion/Net/MaintenanceThread.hpp"
#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketCtrl.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService {
    MZ_DISALLOW_COPY_AND_ASSIGN(NetworkService)

    friend class IoThread;
    friend class MaintenanceThread;

    using Lock_t = std::mutex;
    using LockGuard_t = std::lock_guard<Lock_t>;

public:
    NetworkService();

    virtual ~NetworkService();

public:
    virtual bool Initialize(int socketCapacity, int ioThreadNumber);

    virtual void Finalize();

    bool Attach(Socket* socket);

    bool CanAttachMore() const;

    void Lock();

    void Unlock();

    int GetSocketCapacity() const;

    int GetSocketNumber() const;

    int GetIoThreadNumber() const;

    Socket* GetSocket(int index);

private:
    SocketService& GetSocketService();

    SocketCtrl& GetSocketCtrl(int index);

    void AddSocketCtrl(int index, Socket* socket);

    void RemoveSocketCtrl(int index);

    int GetVacantIndex() const;

    void MarkSocketActive(int index);

    void MarkSocketClosed(int index);

private:
    Lock_t m_lock;
    SocketService m_socketService;
    int m_socketCapacity;
    int m_socketNumber;
    SocketCtrl* m_socketCtrlList;
    int m_ioThreadNumber;
    IoThread** m_ioThreadList;
    MaintenanceThread* m_maintenanceThread;
};

inline bool NetworkService::CanAttachMore() const {
    return m_socketNumber < m_socketCapacity;
}

inline void NetworkService::Lock() {
    m_lock.lock();
}

inline void NetworkService::Unlock() {
    m_lock.unlock();
}

inline int NetworkService::GetSocketCapacity() const {
    return m_socketCapacity;
}

inline int NetworkService::GetSocketNumber() const {
    return m_socketNumber;
}

inline int NetworkService::GetIoThreadNumber() const {
    return m_ioThreadNumber;
}

inline Socket* NetworkService::GetSocket(int index) {
    return m_socketCtrlList[index].m_socket;
}

inline SocketService& NetworkService::GetSocketService() {
    return m_socketService;
}

inline SocketCtrl& NetworkService::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NETWORKSERVICE_HPP_
