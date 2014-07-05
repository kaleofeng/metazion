#ifndef _MZ_NET_IOCPSERVICE_HPP_
#define _MZ_NET_IOCPSERVICE_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/SocketDefine.hpp"

#if defined(MZ_PLATFORM_WINOWS)

DECL_NAMESPACE_MZ_NET_BEGIN

class Socket;

class IocpService {
    DISALLOW_COPY_AND_ASSIGN(IocpService)

public:
    IocpService();

    ~IocpService();

public:
    bool Initialize(int socketCapacity, int ioThreadNumber);

    void Finalize();

    bool Associate(Socket* socket);

    HANDLE GetIocpHandle() const;

private:
    int m_socketCapacity;
    int m_ioThreadNumber;
    HANDLE m_hIocp;
};

inline HANDLE IocpService::GetIocpHandle() const {
    return m_hIocp;
}

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPSERVICE_HPP_
