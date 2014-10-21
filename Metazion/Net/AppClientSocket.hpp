#ifndef _MZ_NET_APPCLIENTSOCKET_HPP_
#define _MZ_NET_APPCLIENTSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalClientSocket.hpp"
#include "Metazion/Net/PacketCodec.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppClientSocket
    : public NormalClientSocket {
    DISALLOW_COPY_AND_ASSIGN(AppClientSocket)

public:
    AppClientSocket();

    virtual ~AppClientSocket();

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

#endif // _MZ_NET_APPCLIENTSOCKET_HPP_
