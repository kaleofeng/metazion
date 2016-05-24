#include "Metazion/Net/EpollIoThread.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include <Metazion/Share/Log/Logger.hpp>

#include "Metazion/Net/NetworkService.hpp"
#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

EpollIoThread::EpollIoThread() {}

EpollIoThread::~EpollIoThread() {}

void EpollIoThread::Initialize(NetworkService* networkService, int index) {
    m_stopDesired = false;
    m_networkService = networkService;
    m_index = index;
    const auto startIndex = m_networkService->GetSocketService().GetStartIndex(m_index);
    m_socketCount = m_networkService->GetSocketService().GetSocketCount(m_index);
    m_socketCtrlList = &m_networkService->GetSocketCtrl(startIndex);
    m_epollfd = m_networkService->GetSocketService().GetEpollfd(m_index);
    m_eventList = &m_networkService->GetSocketService().GetEpollEvent(startIndex);
}

void EpollIoThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void EpollIoThread::Execute() {
    while (!m_stopDesired) {
        ProcessEvents();
    }
}

void EpollIoThread::ProcessEvents() {
    const auto count = epoll_wait(m_epollfd, m_eventList, m_socketCount, 20);
    if (count == 0) {
        return;
    }

    if (count < 0) {
        const auto error = SAGetLastError();
        if (error != EINTR) {
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Warning: epoll wait, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
        }
        return;
    }

    for (auto index = 0; index < count; ++index)  {
        auto& event = m_eventList[index];
        auto socket = static_cast<Socket*>(event.data.ptr);

        if (event.events & EPOLLRDHUP) {
            NS_SHARE::Log(MZ_LOG_DEBUG, "Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            socket->Close();
            continue;
        }

        if (event.events & EPOLLIN) {
            socket->GetIoStrategy().PostInput();
        }

        if (event.events & EPOLLOUT) {
            socket->GetIoStrategy().EnableOutput();
            socket->GetIoStrategy().PostOutput();
        }
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif
