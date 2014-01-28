#ifndef _MZ_NET_APPLISTENSOCKET_HPP_
#define _MZ_NET_APPLISTENSOCKET_HPP_

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

#endif // _MZ_NET_APPLISTENSOCKET_HPP_
