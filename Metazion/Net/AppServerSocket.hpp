#ifndef _MZ_NET_APPSERVERSOCKET_HPP_
#define _MZ_NET_APPSERVERSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalServerSocket.hpp"
#include "Metazion/Net/PacketCodec.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppServerSocket 
    : public NormalServerSocket {
    DISALLOW_COPY_AND_ASSIGN(AppServerSocket)

public:
    AppServerSocket();

    virtual ~AppServerSocket();

public:
    void Dispatch() override;

    bool SendData(int command, const void* data, int length);

    void PostData(int command, const void* data, int length);

protected:
    virtual void OnValidPacket(int command, const void* data, int length) = 0;

    virtual void OnInvalidPacket() = 0;

private:
    EncodeBuffer& GetEncodeBuffer();

    DecodeBuffer& GetDecodeBuffer();

private:
    PacketCodec m_packetCodec;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSERVERSOCKET_HPP_
