#ifndef _MZ_NET_SOCKETCTRL_HPP_
#define _MZ_NET_SOCKETCTRL_HPP_

#include "Metazion/Net/NetInclude.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;

struct SocketCtrl {
    Socket* m_socket = nullptr;
    bool m_active = false;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETCTRL_HPP_
