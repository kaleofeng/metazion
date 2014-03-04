#ifndef _MZ_NET_APPSERVERSOCKET_HPP_
#define _MZ_NET_APPSERVERSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalServerSocket.hpp"
#include "Metazion/Net/PacketSpecific.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppServerSocket 
    : public NormalServerSocket {
    DISALLOW_COPY_AND_ASSIGN(AppServerSocket)

public:
    AppServerSocket();

    virtual ~AppServerSocket();

public:
    void Dispatch() override;

    virtual void OnValidPacket(int command, const void* data, int length) = 0;

    virtual void OnInvalidPacket() = 0;

    bool SendData(int command, const void* data, int length);

private:
    EncodeBuffer& GetEncodeBuffer();

    DecodeBuffer& GetDecodeBuffer();

private:
    PacketSpecific m_packetSpecific;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSERVERSOCKET_HPP_
