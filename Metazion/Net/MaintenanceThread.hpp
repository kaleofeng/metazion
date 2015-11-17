#ifndef _MZ_NET_MAINTENANCETHREAD_HPP_
#define _MZ_NET_MAINTENANCETHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <atomic>

#include <Metazion/Share/Thread/Thread.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;
class NetworkService;

class MaintenanceThread : public NS_SHARE::Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(MaintenanceThread)

private:
    NetworkService* m_networkService = nullptr;
    int m_interval = 0;
    std::atomic<bool> m_stopDesired = { false };

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
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_MAINTENANCETHREAD_HPP_
