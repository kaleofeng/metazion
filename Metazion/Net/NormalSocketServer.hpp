#ifndef _MZ_NET_NORMALSOCKETSERVER_HPP_
#define _MZ_NET_NORMALSOCKETSERVER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/GenericSocketServer.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NormalSocketServer : public GenericSocketServer {
    DISALLOW_COPY_AND_ASSIGN(NormalSocketServer)
    
public:
    NormalSocketServer();

    virtual ~NormalSocketServer();
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_NORMALSOCKETSERVER_HPP_
