#ifndef _MZ_NET_MAINTENANCETHREAD_HPP_
#define _MZ_NET_MAINTENANCETHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Thread/Thread.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;
class NetworkService;

class MaintenanceThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(MaintenanceThread)

public:
    MaintenanceThread();

    ~MaintenanceThread();

public:
    void Init(NetworkService* networkService);

    void Finalize();

protected:
    void Execute() override final;

private:
    void ProcessSockets();

    void ProcessActiveSocket(Socket* socket, int index);

    void ProcessClosedSocket(Socket* socket, int index);

private:
    NetworkService* m_networkService;
    int m_interval;
    volatile bool m_stopDesired;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_MAINTENANCETHREAD_HPP_
