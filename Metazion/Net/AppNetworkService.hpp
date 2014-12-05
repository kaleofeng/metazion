#ifndef _MZ_NET_APPNETWORKSERVICE_HPP_
#define _MZ_NET_APPNETWORKSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/StaticArray.hpp>

#include "Metazion/Net/NormalNetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppNetworkService : public NormalNetworkService {
    DISALLOW_COPY_AND_ASSIGN(AppNetworkService)

public:
    using SocketArray_t = NS_SHARE::StaticArray<Socket*>;

    struct SocketFilter {
        virtual bool Filter(Socket* socket) { 
            return socket->GetType() != SOCKET_TCP_LISTEN;
        }
    };
    
public:
    AppNetworkService();

    virtual ~AppNetworkService();

public:
    int LockSockets(SocketFilter& filter, SocketArray_t& socketArray);

    void UnlockSockets(SocketArray_t& socketArray);
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPNETWORKSERVICE_HPP_
