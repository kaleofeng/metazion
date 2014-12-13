#ifndef _MZ_NET_NORMALNETWORKSERVICE_HPP_
#define _MZ_NET_NORMALNETWORKSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/GenericNetworkService.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalNetworkService : public GenericNetworkService {
    MZ_DISALLOW_COPY_AND_ASSIGN(NormalNetworkService)
    
public:
    NormalNetworkService();

    virtual ~NormalNetworkService();
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NORMALNETWORKSERVICE_HPP_
