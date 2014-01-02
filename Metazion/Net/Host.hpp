#ifndef MZ_NET_HOST_HPP
#define MZ_NET_HOST_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Host {
public:
    Host() { 
        ::memset(&m_sockAddrIn, 0, sizeof(m_sockAddrIn)); 
    }

    ~Host() {}

    Host(const char *ip, int port, int family = AF_INET) {
        m_sockAddrIn.sin_family = family;
        m_sockAddrIn.sin_addr.s_addr = inet_addr(ip);
        m_sockAddrIn.sin_port = htons(port);
    }

    Host& operator =(const Host& other) {
        if(this != &other) {
            ::memcpy(&m_sockAddrIn, &other.m_sockAddrIn, sizeof(m_sockAddrIn));
        }

        return *this;
    }

public:
    void Reset() {
        ::memset(&m_sockAddrIn, 0, sizeof(m_sockAddrIn));
    }

    SockAddrIn_t* SockAddrIn() {
        return &m_sockAddrIn;
    }

    SockAddr_t* SockAddr() {
        return reinterpret_cast<SockAddr_t*>(&m_sockAddrIn);
    }

    SockLen_t SockAddrLen() const {
        return sizeof(m_sockAddrIn);
    }

    int GetFamily() const {
        return m_sockAddrIn.sin_family;
    }

    void SetFamily(int arg) {
        m_sockAddrIn.sin_family = arg;
    }

    const char* GetIP() const {
        return inet_ntoa(m_sockAddrIn.sin_addr);
    }

    void SetIp(const char* arg) {
        m_sockAddrIn.sin_addr.s_addr = inet_addr(arg);
    }

    int GetPort() const {
        return m_sockAddrIn.sin_port;
    }

    void SetPort(int arg) {
        m_sockAddrIn.sin_port = htons(arg);
    }

private:
    SockAddrIn_t m_sockAddrIn;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_HOST_HPP