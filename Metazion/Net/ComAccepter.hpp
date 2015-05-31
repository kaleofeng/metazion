#ifndef _MZ_NET_COMACCEPTER_HPP_
#define _MZ_NET_COMACCEPTER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <functional>

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;
class AppServerSocket;

class ComAccepter {
    MZ_DISALLOW_COPY_AND_ASSIGN(ComAccepter)

    using CreateSocketCallback_t = std::function<AppServerSocket*()>;

public:
    ComAccepter(ListenSocket& socket);

    ~ComAccepter();

public:
    void Reset();

    bool Accept(const SockId_t& sockId);

    void SetCreateSocketCallback(CreateSocketCallback_t callback);

private:
    ListenSocket& m_socket;

    CreateSocketCallback_t m_createServerSocketCallback;
};

inline void ComAccepter::SetCreateSocketCallback(CreateSocketCallback_t callback) {
    m_createServerSocketCallback = callback;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETACCEPTER_HPP_
