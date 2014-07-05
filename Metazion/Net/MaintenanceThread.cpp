#include "Metazion/Net/MaintenanceThread.hpp"

#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

MaintenanceThread::MaintenanceThread()
    : m_socketServer(nullptr)
    , m_interval(0)
    , m_stopDesired(false) {}

MaintenanceThread::~MaintenanceThread() {}

void MaintenanceThread::Init(SocketServer* socketServer) {
    ASSERT_TRUE(!IsNull(socketServer));

    m_socketServer = socketServer;
    m_interval = 0;
    m_stopDesired = false;
}

void MaintenanceThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void MaintenanceThread::Execute() {
    auto lastTime = NS_SHARE::GetNowMillisecond();
    auto lastTickTime = lastTime;
    while (!m_stopDesired) {
        const auto curTime = NS_SHARE::GetNowMillisecond();
        if (curTime - lastTime < 10) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        m_interval = static_cast<int>(curTime - lastTickTime);
        lastTickTime = curTime;

        ProcessSockets();

        lastTime = curTime;
    }
}

void MaintenanceThread::ProcessSockets() {
    const auto socketCapacity = m_socketServer->GetSocketCapacity();
    for (auto index = 0; index < socketCapacity; ++index) {
        auto& socketCtrl = m_socketServer->GetSocketCtrl(index);
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (socketCtrl.m_active) {
            ProcessActiveSocket(socketCtrl.m_socket, index);
        }
        else {
            ProcessClosedSocket(socketCtrl.m_socket, index);
        }
    }
}

void MaintenanceThread::ProcessActiveSocket(Socket* socket, int index) {
    if (socket->IsClosed()) {
        m_socketServer->MarkSocketClosed(index);
        return;
    }

    socket->Tick(m_interval);

    socket->GetIoStrategy().Output();
}

void MaintenanceThread::ProcessClosedSocket(Socket* socket, int index) {
    if (socket->IsActive()) {
        m_socketServer->MarkSocketActive(index);
        return;
    }

    socket->Tick(m_interval);

    if (!socket->IsAlive()) {
        m_socketServer->Lock();
        if (socket->IsAlive()) {
            m_socketServer->Unlock();
            return;
        }

        m_socketServer->RemoveSocketCtrl(index);
        m_socketServer->Unlock();

        socket->OnDetached();
        socket->Destory();
    }
}

DECL_NAMESPACE_MZ_NET_END
