#ifndef MZ_NET_NETWORK_HPP
#define MZ_NET_NETWORK_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Host.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Network {
public:
    static int Startup();

    static int Cleanup();
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_NETWORK_HPP
