#ifndef MZ_NET_APPLISTENSOCKET_HPP
#define MZ_NET_APPLISTENSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalListenSocket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppListenSocket : public NormalListenSocket {
    DISALLOW_COPY_AND_ASSIGN(AppListenSocket)

public:
    AppListenSocket();

    virtual ~AppListenSocket();
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_APPLISTENSOCKET_HPP
