#ifndef _MZ_NET_SOCKETSERVER_HPP_
#define _MZ_NET_SOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Sync/AutoGuard.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>
#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/AlternativeService.hpp"
#include "Metazion/Net/AlternativeThread.hpp"
#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketCtrl.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class SocketServer {
    DISALLOW_COPY_AND_ASSIGN(SocketServer)

    friend class IoThread;
    friend class MaintenanceThread;

public:
    SocketServer();

    virtual ~SocketServer();

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
    NS_SHARE::MutexLock m_lock;
    SocketService m_socketService;
    int m_socketCapacity;
    int m_socketNumber;
    SocketCtrl* m_socketCtrlList;
    int m_ioThreadNumber;
    IoThread** m_ioThreadList;
    MaintenanceThread* m_maintenanceThread;
};

inline bool SocketServer::CanAttachMore() const {
    return m_socketNumber < m_socketCapacity;
}

inline void SocketServer::Lock() {
    m_lock.Lock();
}

inline void SocketServer::Unlock() {
    m_lock.Unlock();
}

inline int SocketServer::GetSocketCapacity() const {
    return m_socketCapacity;
}

inline int SocketServer::GetSocketNumber() const {
    return m_socketNumber;
}

inline int SocketServer::GetIoThreadNumber() const {
    return m_ioThreadNumber;
}

inline Socket* SocketServer::GetSocket(int index) {
    return m_socketCtrlList[index].m_socket;
}

inline SocketService& SocketServer::GetSocketService() {
    return m_socketService;
}

inline SocketCtrl& SocketServer::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETSERVER_HPP_
