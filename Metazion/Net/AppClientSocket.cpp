#include "Metazion/Net/AppClientSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

AppClientSocket::AppClientSocket()
    : m_connecter(*this)
    , m_packeter(*this) {}

AppClientSocket::~AppClientSocket() {}

void AppClientSocket::Reset() {
    TransmitSocket::Reset();
    
    m_connecter.Reset();
    m_packeter.Reset();

    DerivedReset();
}

void AppClientSocket::Prepare() {
    TransmitSocket::Prepare();

    DerivedPrepare();
}

void AppClientSocket::Tick(int64_t now, int interval) {
    TransmitSocket::Tick(now, interval);
    
    m_connecter.Tick(now, interval);

    DerivedTick(now, interval);
}

void AppClientSocket::Dispatch() {
    TransmitSocket::Dispatch();

    m_packeter.Dispatch();

    DerivedDispatch();
}

void AppClientSocket::OnConnected() {
    TransmitSocket::OnConnected();

    DerivedOnConnected();
}

void AppClientSocket::OnDisconnected() {
    TransmitSocket::OnDisconnected();

    DerivedOnDisconnected();
}

void AppClientSocket::OnSended(const void* data, int length) {
    TransmitSocket::OnSended(data, length);

    DerivedOnSended(data, length);
}

void AppClientSocket::OnRecved(const void* data, int length) {
    TransmitSocket::OnRecved(data, length);

    DerivedOnRecved(data, length);
}

DECL_NAMESPACE_MZ_NET_END
