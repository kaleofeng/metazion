#include "Metazion/Net/AppServerSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

AppServerSocket::AppServerSocket()
    : m_packeter(*this) {}

AppServerSocket::~AppServerSocket() {}

void AppServerSocket::Reset() {
    TransmitSocket::Reset();

    m_packeter.Reset();

    DerivedReset();
}

void AppServerSocket::Prepare() {
    TransmitSocket::Prepare();

    DerivedPrepare();
}

void AppServerSocket::Tick(int64_t now, int interval) {
    TransmitSocket::Tick(now, interval);

    DerivedTick(now, interval);
}

void AppServerSocket::Dispatch() {
    TransmitSocket::Dispatch();

    m_packeter.Dispatch();

    DerivedDispatch();
}

void AppServerSocket::Attach(const SockId_t& sockId) {
    AttachSockId(sockId);
}

void AppServerSocket::OnConnected() {
    TransmitSocket::OnConnected();

    DerivedOnConnected();
}

void AppServerSocket::OnDisconnected() {
    TransmitSocket::OnDisconnected();

    DerivedOnDisconnected();
}

void AppServerSocket::OnSended(const void* data, int length) {
    TransmitSocket::OnSended(data, length);

    DerivedOnSended(data, length);
}

void AppServerSocket::OnRecved(const void* data, int length) {
    TransmitSocket::OnRecved(data, length);

    DerivedOnRecved(data, length);
}

DECL_NAMESPACE_MZ_NET_END
