#include "Metazion/Net/EpollSocket.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_NET_BEGIN

EpollSocket::EpollSocket()
    : m_canOutput(false) {}

EpollSocket::~EpollSocket() {}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX
