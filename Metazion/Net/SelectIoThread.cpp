#include "Metazion/Net/SelectIoThread.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/NetworkService.hpp"
#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

SelectIoThread::SelectIoThread() {}

SelectIoThread::~SelectIoThread() {}

void SelectIoThread::Initialize(NetworkService* networkService, int index) {
    m_stopDesired = false;
    m_networkService = networkService;
    m_index = index;
    const auto startIndex = m_networkService->GetSocketService().GetStartIndex(m_index);
    m_socketCount = m_networkService->GetSocketService().GetSocketCount(m_index);
    m_socketCtrlList = &m_networkService->GetSocketCtrl(startIndex);
}

void SelectIoThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void SelectIoThread::Execute() {
    while (!m_stopDesired) {
        ProcessEvents();
        ProcessIO();
    }
}

void SelectIoThread::ProcessEvents() {
    const auto maxFd = ResetFds();
    if (maxFd < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return;
    }

    struct timeval timeout{ 0, 20 * 1000 };
    const auto ret = select(maxFd + 1, &m_rfds, &m_wfds, &m_efds, &timeout);
    if (ret == 0) {
        return;
    }
    else if (ret < 0) {
        return;
    }

    for (auto index = 0; index < m_socketCount; ++index)  {
        const auto& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (!socketCtrl.m_active) {
            continue;
        }

        const auto socket = socketCtrl.m_socket;
        const auto sockId = socket->GetSockId();
        if (FD_ISSET(sockId, &m_efds)) {
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            socket->Close();
            continue;
        }

        if (FD_ISSET(sockId, &m_rfds)) {
            socket->TheIoStrategy().EnableInput();
        }

        if (FD_ISSET(sockId, &m_wfds)) {
            socket->TheIoStrategy().EnableOutput();
        }
    }
}

void SelectIoThread::ProcessIO() {
    for (int index = 0; index < m_socketCount; ++index) {
        const auto& socketCtrl = m_socketCtrlList[index];
        const auto socket = socketCtrl.m_socket;
        if (IsNull(socket)) {
            continue;
        }

        socket->TheIoStrategy().PostInput();
        socket->TheIoStrategy().PostOutput();
    }
}

int SelectIoThread::ResetFds() {
    FD_ZERO(&m_rfds);
    FD_ZERO(&m_wfds);
    FD_ZERO(&m_efds);

    int maxFd = -1;
    for (auto index = 0; index < m_socketCount; ++index) {
        const auto& socketCtrl = m_socketCtrlList[index];
        const auto socket = socketCtrl.m_socket;
        if (IsNull(socket)) {
            continue;
        }

        if (!socket->IsWorking()) {
            continue;
        }

        const auto sockId = socket->GetSockId();
        FD_SET(sockId, &m_rfds);
        FD_SET(sockId, &m_efds);
        if (socket->TheIoStrategy().ShouldCareAboutOutput()) {
            FD_SET(sockId, &m_wfds);
        }

        const auto sockFd = static_cast<int>(sockId);
        maxFd = sockFd > maxFd ? sockFd : maxFd;
    }

    return maxFd;
}

DECL_NAMESPACE_MZ_NET_END

#endif
