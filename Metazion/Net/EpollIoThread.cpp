#include "Metazion/Net/EpollIoThread.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/Socket.hpp"
#include "Metazion/Net/SocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

EpollIoThread::EpollIoThread()
    : m_socketServer(nullptr)
    , m_index(-1)
    , m_epollfd(0)
    , m_eventList(nullptr)
    , m_socketCount(0)
    , m_socketCtrlList(nullptr)
    , m_stopDesired(false) {}

EpollIoThread::~EpollIoThread() {}

void EpollIoThread::Initialize(SocketServer* socketServer, int index) {
    m_socketServer = socketServer;
    m_index = index;
    const auto startIndex = m_socketServer->GetSocketService().GetStartIndex(m_index);
    m_epollfd = m_socketServer->GetSocketService().GetEpollfd(m_index);
    m_eventList = &m_socketServer->GetSocketService().GetEpollEvent(startIndex);
    m_socketCount = m_socketServer->GetSocketService().GetSocketCount(m_index);
    m_socketCtrlList = &m_socketServer->GetSocketCtrl(startIndex);
    m_stopDesired = false;
}

void EpollIoThread::Finalize() {
    m_stopDesired = true;
    Join();
}

void EpollIoThread::Execute() {
    while (!m_stopDesired) {
        ProcessSockets();
        ProcessEvents();
    }
}

void EpollIoThread::ProcessSockets() {
    for (auto index = 0; index < m_socketCount; ++index) {
        auto& socketCtrl = m_socketCtrlList[index];
        if (IsNull(socketCtrl.m_socket)) {
            continue;
        }

        if (!socketCtrl.m_active) {
            continue;
        }

        socketCtrl.m_socket->GetIoStrategy().ExecuteOutput();
    }
}

void EpollIoThread::ProcessEvents() {
    const auto count = ::epoll_wait(m_epollfd, m_eventList, m_socketCount, 10);
    if (0 == count) {
        return;
    }

    if (count < 0) {
        const auto error = GetLastError();
        if (EINTR != error) {
            ::printf("Socket Warning: epoll wait, error[%d]. [%s:%d]\n", error, __FILE__, __LINE__);
        }
        return;
    }

    for (auto index = 0; index < count; ++index)  {
        auto& event = m_eventList[index];
        auto socket = static_cast<Socket*>(event.data.ptr);

        if (event.events & EPOLLRDHUP) {
            ::printf("Socket Info: socket close. [%s:%d]\n", __FILE__, __LINE__);
            socket->Close();
            continue;
        }

        if (event.events & EPOLLIN) {
            socket->GetIoStrategy().ExecuteInput();
        }

        if (event.events & EPOLLOUT) {
            socket->GetIoStrategy().EnableOutput();
        }
    }
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
