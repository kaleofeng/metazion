#ifndef _MZ_NET_ADDRESS_HPP_
#define _MZ_NET_ADDRESS_HPP_

#include "Metazion/Net/NetInclude.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

struct Address {
    UINT32 m_ip = 0;
    UINT16 m_port = 0;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_ADDRESS_HPP_
