#ifndef _MZ_NET_APPNETWORKSERVICE_HPP_
#define _MZ_NET_APPNETWORKSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Collection/StaticArray.hpp>

#include "Metazion/Net/GenericNetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppNetworkService : public GenericNetworkService {
    MZ_DISALLOW_COPY_AND_ASSIGN(AppNetworkService)

public:
    using SocketArray_t = NS_SHARE::StaticArray<Socket*>;

public:
    AppNetworkService();

    virtual ~AppNetworkService();

public:
    int LockSockets(std::function<bool(Socket*)> socketFilter, SocketArray_t& socketArray);

    void UnlockSockets(SocketArray_t& socketArray);
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPNETWORKSERVICE_HPP_
