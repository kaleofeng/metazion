#include "Metazion/Net/MaintenanceThread.hpp"

#include <Metazion/Share/Time/Time.hpp>

#include "Metazion/Net/NetworkService.hpp"
#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

MaintenanceThread::MaintenanceThread() {}

MaintenanceThread::~MaintenanceThread() {}

void MaintenanceThread::Init(NetworkService* networkService) {
    MZ_ASSERT_TRUE(!IsNull(networkService));

    m_stopDesired = false;
    m_networkService = networkService;
    m_now = 0;
    m_interval = 0;
}

void MaintenanceThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void MaintenanceThread::Execute() {
    auto lastTime = NS_SHARE::GetNowMillisecond();
    auto lastTickTime = lastTime;
    while (!m_stopDesired) {
        m_now = NS_SHARE::GetNowMillisecond();
        if (m_now - lastTime < 10) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        m_interval = static_cast<int>(m_now - lastTickTime);
        lastTickTime = m_now;

        ProcessSockets();

        lastTime = m_now;
    }
}

void MaintenanceThread::ProcessSockets() {
    const auto socketCapacity = m_networkService->GetSocketCapacity();
    for (auto index = 0; index < socketCapacity; ++index) {
        auto& socketCtrl = m_networkService->GetSocketCtrl(index);
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        socketCtrl.m_socket->Tick(m_now, m_interval);

        socketCtrl.m_active
            ? ProcessActiveSocket(socketCtrl.m_socket, index)
            : ProcessClosedSocket(socketCtrl.m_socket, index);
    }
}

void MaintenanceThread::ProcessActiveSocket(Socket* socket, int index) {
    if (socket->KeepEnough()) {
        socket->Disconnect();
    }

    if (socket->IsGonnaClose()) {
        socket->Stop();
    }

    if (!socket->IsActive()) {
        m_networkService->MarkSocketClosed(index);
    }
}

void MaintenanceThread::ProcessClosedSocket(Socket* socket, int index) {
    if (socket->IsActive()) {
        m_networkService->MarkSocketActive(index);
        return;
    }

    if (!socket->IsAlive()) {
        m_networkService->Lock();
        if (socket->IsAlive()) {
            m_networkService->Unlock();
            return;
        }

        m_networkService->RemoveSocketCtrl(index);
        m_networkService->Unlock();

        socket->OnDetached();
        socket->Destory();
    }
}

DECL_NAMESPACE_MZ_NET_END
