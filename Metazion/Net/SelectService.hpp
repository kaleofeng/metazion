#ifndef _MZ_NET_SELECTSERVICE_HPP_
#define _MZ_NET_SELECTSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/SocketDefine.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;

class SelectService {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectService)

private:
    int m_socketCapacity = 0;
    int m_ioThreadNumber = 0;

public:
    SelectService();

    ~SelectService();

public:
    bool Initialize(int socketCapacity, int ioThreadNumber);

    void Finalize();

    bool Associate(Socket* socket);

    int GetThreadIndex(int socketIndex);

    int GetStartIndex(int threadIndex) const;

    int GetSocketCount(int threadIndex) const;
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTSERVICE_HPP_
