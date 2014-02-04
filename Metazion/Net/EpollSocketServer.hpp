#ifndef _MZ_NET_EPOLLSOCKETSERVER_HPP_
#define _MZ_NET_EPOLLSOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include <Metazion/Share/Sync/AutoGuard.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>
#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class EpollIoThread;
class EpollMaintenanceThread;

class EpollSocketServer : public SocketServer {
    DISALLOW_COPY_AND_ASSIGN(EpollSocketServer)

    friend class EpollIoThread;
    friend class EpollMaintenanceThread;

public:
    struct SocketCtrl {
        SocketCtrl()
            : m_socket(nullptr)
            , m_active(false) {}

        Socket* m_socket;
        bool m_active;
    };

public:
    EpollSocketServer();

    virtual ~EpollSocketServer();

public:
    bool Initialize(int socketCapacity, int ioThreadNumber) override;

    void Finalize() override;

    bool Attach(Socket* socket) override;

    bool CanAttachMore() const override;

    void Lock();

    void Unlock();

    int GetSocketCapacity() const;

    int GetIoThreadNumber() const;

    int GetSocketCount() const;

    Socket* GetSocket(int index);

private:
    void MarkSocketActive(int index);

    void MarkSocketClosed(int index);

    bool AssociateWithEpoll(Socket* socket);

    int GetEpollfd(int threadIndex) const;

    struct epoll_event& GetEpollEvent(int index);

    SocketCtrl& GetSocketCtrl(int index);

    void AddSocketCtrl(int index, Socket* socket);

    void RemoveSocketCtrl(int index);

    int GetVacantIndex() const;

    int GetThreadIndex(int socketIndex);

    int GetStartIndex(int threadIndex) const;

    int GetSocketCount(int threadIndex) const;

private:
    NS_SHARE::MutexLock m_lock;
    int* m_epollfdList;
    struct epoll_event* m_epollEventList;
    int m_socketCapacity;
    int m_socketCount;
    SocketCtrl* m_socketCtrlList;
    int m_ioThreadNumber;
    EpollIoThread** m_ioThreadList;
    EpollMaintenanceThread* m_maintenanceThread;
};

inline bool EpollSocketServer::CanAttachMore() const {
    return m_socketCount < m_socketCapacity;
}

inline void EpollSocketServer::Lock() {
    m_lock.Lock();
}

inline void EpollSocketServer::Unlock() {
    m_lock.Unlock();
}

inline int EpollSocketServer::GetSocketCapacity() const {
    return m_socketCapacity;
}

inline int EpollSocketServer::GetIoThreadNumber() const {
    return m_ioThreadNumber;
}

inline int EpollSocketServer::GetSocketCount() const {
    return m_socketCount;
}

inline Socket* EpollSocketServer::GetSocket(int index) {
    return m_socketCtrlList[index].m_socket;
}

inline int EpollSocketServer::GetEpollfd(int threadIndex) const {
    return m_epollfdList[threadIndex];
}

inline struct epoll_event& EpollSocketServer::GetEpollEvent(int index) {
    return m_epollEventList[index];
}

inline EpollSocketServer::SocketCtrl& EpollSocketServer::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

class EpollIoThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(EpollIoThread)

public:
    EpollIoThread();

    ~EpollIoThread();

public:
    void Initialize(EpollSocketServer* socketServer, int index);

    void Finalize();

protected:
    void Stop();

    void Execute();

private:
    void ProcessSockets();

    void ProcessEvents();

private:
    EpollSocketServer* m_socketServer;
    int m_index;
    int m_epollfd;
    struct epoll_event* m_eventList;
    int m_socketCount;
    EpollSocketServer::SocketCtrl* m_socketCtrlList;
    volatile bool m_stopDesired;
};

class EpollMaintenanceThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(EpollMaintenanceThread)

public:
    EpollMaintenanceThread();

    ~EpollMaintenanceThread();

public:
    void Init(EpollSocketServer* socketServer);

    void Finalize();

protected:
    void Stop();

    void Execute();

private:
    void ProcessSockets();

    void ProcessActiveSocket(Socket* socket, int index);

    void ProcessClosedSocket(Socket* socket, int index);

private:
    EpollSocketServer* m_socketServer;
    int m_interval;
    volatile bool m_stopDesired;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_NET_EPOLLSOCKETSERVER_HPP_
