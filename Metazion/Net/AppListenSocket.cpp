#include "Metazion/Net/AppListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

AppListenSocket::AppListenSocket()
    : m_accepter(*this) {}

AppListenSocket::~AppListenSocket() {}

void AppListenSocket::Reset() {
    ListenSocket::Reset();

    m_accepter.Reset();

    DerivedReset();
}

void AppListenSocket::Prepare() {
    ListenSocket::Prepare();

    DerivedPrepare();
}

void AppListenSocket::Tick(int interval) {
    ListenSocket::Tick(interval);

    DerivedTick(interval);
}

void AppListenSocket::Dispatch() {
    ListenSocket::Dispatch();

    DerivedDispatch();
}

void AppListenSocket::OnWatched() {
    ListenSocket::OnWatched();

    DerivedOnWatched();
}

void AppListenSocket::OnUnwatched() {
    ListenSocket::OnUnwatched();

    DerivedOnUnwatched();
}

bool AppListenSocket::OnAccepted(const SockId_t& sockId) {
    auto ret = ListenSocket::OnAccepted(sockId);
    if (!ret) {
        return false;
    }

    ret = m_accepter.Accept(sockId);
    if (!ret) {
        return false;
    }

    return DerivedOnAccepted(sockId);
}

DECL_NAMESPACE_MZ_NET_END
