#ifndef _MZ_NET_COMACCEPTER_HPP_
#define _MZ_NET_COMACCEPTER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <functional>

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;
class AppServerSocket;
class Host;

class ComAccepter {
    MZ_DISALLOW_COPY_AND_ASSIGN(ComAccepter)

    using CreateSocketCallback_t = std::function<AppServerSocket*()>;

private:
    ListenSocket& m_socket;

    CreateSocketCallback_t m_createServerSocketCallback;

public:
    ComAccepter(ListenSocket& socket);

    ~ComAccepter();

    void Reset();

    bool Accept(const SockId_t& sockId, const Host& host);

    void SetCreateSocketCallback(CreateSocketCallback_t callback);
};

inline void ComAccepter::SetCreateSocketCallback(CreateSocketCallback_t callback) {
    m_createServerSocketCallback = callback;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_SOCKETACCEPTER_HPP_
