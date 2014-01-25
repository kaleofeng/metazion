#ifndef MZ_NET_IOCPSOCKETSERVER_HPP
#define MZ_NET_IOCPSOCKETSERVER_HPP

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

    bool AssociateWithIocp(Socket* socket);

    int GetVacantIndex() const;

    HANDLE GetIocpHandle() const {
        return m_hIocp;
    }

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
    void Stop();

    void Execute();

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
    void Stop();

    void Execute();

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

#endif // MZ_NET_IOCPSOCKETSERVER_HPP
