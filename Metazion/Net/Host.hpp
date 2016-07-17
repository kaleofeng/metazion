#ifndef _MZ_NET_HOST_HPP_
#define _MZ_NET_HOST_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/Address.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class Host {

private:
    SockAddrIn_t m_sockAddrIn;
    char m_buffer[64] = { '\0' };

public:
    Host() { 
        memset(&m_sockAddrIn, 0, sizeof(m_sockAddrIn)); 
    }

    ~Host() {}

    Host(const Host& other) {
        *this = other;
    }

    Host& operator =(const Host& other) {
        if(this != &other) {
            memcpy(&m_sockAddrIn, &other.m_sockAddrIn, sizeof(m_sockAddrIn));
        }

        return *this;
    }

public:
    void Reset() {
        memset(&m_sockAddrIn, 0, sizeof(m_sockAddrIn));
    }

    void FromSockAddrIn(const SockAddrIn_t* sockAddrIn) {
        memcpy(&m_sockAddrIn, sockAddrIn, sizeof(m_sockAddrIn));
    }

    void FromAddress(const Address& address, int family = AF_INET) {
        m_sockAddrIn.sin_family = family;
        m_sockAddrIn.sin_addr.s_addr = htonl(address.m_ip);
        m_sockAddrIn.sin_port = htons(address.m_port);
    }

    Address ToAddress() {
        Address address;
        address.m_ip = ntohl(m_sockAddrIn.sin_addr.s_addr);
        address.m_port = ntohs(m_sockAddrIn.sin_port);
        return address;
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

    void SetFamily(int family) {
        m_sockAddrIn.sin_family = family;
    }

    const char* GetIp() {
        return inet_ntop(m_sockAddrIn.sin_family, &m_sockAddrIn.sin_addr, m_buffer, sizeof(m_buffer));
    }

    void SetIp(const char* ip) {
        inet_pton(m_sockAddrIn.sin_family, ip, &m_sockAddrIn.sin_addr);
    }

    int GetPort() const {
        return ntohs(m_sockAddrIn.sin_port);
    }

    void SetPort(int port) {
        m_sockAddrIn.sin_port = htons(port);
    }
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_HOST_HPP_
