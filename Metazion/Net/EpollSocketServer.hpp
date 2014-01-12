#ifndef MZ_NET_EPOLLSOCKETSERVER_HPP
#define MZ_NET_EPOLLSOCKETSERVER_HPP

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

public: // @Override
    bool Initialize(int socketCapacity, int ioThreadNumber);

    void Finalize();

    bool Attach(Socket* socket);

    bool CanAttachMore() const;

public:
    void Lock() {
        m_lock.Lock();
    }

    void Unlock() {
        m_lock.Unlock();
    }

    int GetSocketCapacity() const {
        return m_socketCapacity;
    }

    int GetIoThreadNumber() const {
        return m_ioThreadNumber;
    }

    int GetSocketCount() const {
        return m_socketCount;
    }

    Socket* GetSocket(int index) {
        return m_socketCtrlList[index].m_socket;
    }

private:
    void MarkSocketActive(int index);

    void MarkSocketClosed(int index);

    SocketCtrl& GetSocketCtrl(int index);

    void AddSocketCtrl(int index, Socket* socket);

    void RemoveSocketCtrl(int index);

    int GetEpollfd(int threadIndex) const;

    struct epoll_event& GetEpollEvent(int index);

    bool AssociateWithEpoll(Socket* socket);

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

#endif // MZ_NET_EPOLLSOCKETSERVER_HPP
