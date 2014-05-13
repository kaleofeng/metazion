#ifndef _MZ_NET_IOCPSOCKETSERVER_HPP_
#define _MZ_NET_IOCPSOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <Metazion/Share/Thread/Thread.hpp>
#include <Metazion/Share/Sync/AutoGuard.hpp>
#include <Metazion/Share/Sync/MutexLock.hpp>

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class IocpIoThread;
class IocpMaintenanceThread;

class IocpSocketServer : public SocketServer {
    DISALLOW_COPY_AND_ASSIGN(IocpSocketServer)

    friend class IocpIoThread;
    friend class IocpMaintenanceThread;

public:
    struct SocketCtrl {
        SocketCtrl()
            : m_socket(nullptr)
            , m_active(false) {}

        Socket* m_socket;
        bool m_active;
    };

public:
    IocpSocketServer();

    virtual ~IocpSocketServer();

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

    bool AssociateWithIocp(Socket* socket);

    HANDLE GetIocpHandle() const;

    SocketCtrl& GetSocketCtrl(int index);

    void AddSocketCtrl(int index, Socket* socket);

    void RemoveSocketCtrl(int index);

    int GetVacantIndex() const;

private:
    NS_SHARE::MutexLock m_lock;
    HANDLE m_hIocp;
    int m_socketCapacity;
    int m_socketCount;
    SocketCtrl* m_socketCtrlList;
    int m_ioThreadNumber;
    IocpIoThread** m_ioThreadList;
    IocpMaintenanceThread* m_maintenanceThread;
};

inline bool IocpSocketServer::CanAttachMore() const {
    return m_socketCount < m_socketCapacity;
}

inline void IocpSocketServer::Lock() {
    m_lock.Lock();
}

inline void IocpSocketServer::Unlock() {
    m_lock.Unlock();
}

inline int IocpSocketServer::GetSocketCapacity() const {
    return m_socketCapacity;
}

inline int IocpSocketServer::GetIoThreadNumber() const {
    return m_ioThreadNumber;
}

inline int IocpSocketServer::GetSocketCount() const {
    return m_socketCount;
}

inline Socket* IocpSocketServer::GetSocket(int index) {
    return m_socketCtrlList[index].m_socket;
}

inline HANDLE IocpSocketServer::GetIocpHandle() const {
    return m_hIocp;
}

inline IocpSocketServer::SocketCtrl& IocpSocketServer::GetSocketCtrl(int index) {
    return m_socketCtrlList[index];
}

class IocpIoThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(IocpIoThread)

    enum Result {
        RESULT_SUCCESS = 1,
        RESULT_FAILURE,
        RESULT_CLOSE,
        RESULT_TIMEOUT,
        RESULT_ERROR,
    };

public:
    IocpIoThread();

    ~IocpIoThread();

public:
    void Initialize(IocpSocketServer* socketServer);

    void Finalize();

protected:
    void Execute() override final;

private:
    int AnalyseStatusResult(BOOL ret
        , OVERLAPPED* overlapped
        , DWORD numberOfBytes
        , DWORD error);

private:
    IocpSocketServer* m_socketServer;
    volatile bool m_stopDesired;
};

class IocpMaintenanceThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(IocpMaintenanceThread)

public:
    IocpMaintenanceThread();

    ~IocpMaintenanceThread();

public:
    void Init(IocpSocketServer* socketServer);

    void Finalize();

protected:
    void Execute() override final;

private:
    void ProcessSockets();

    void ProcessActiveSocket(Socket* socket, int index);

    void ProcessClosedSocket(Socket* socket, int index);

private:
    IocpSocketServer* m_socketServer;
    int m_interval;
    volatile bool m_stopDesired;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPSOCKETSERVER_HPP_
