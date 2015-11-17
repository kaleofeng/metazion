#ifndef _MZ_NET_NETWORK_HPP_
#define _MZ_NET_NETWORK_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Host.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Network {

public:
    static int Startup();

    static int Cleanup();
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NETWORK_HPP_
