#ifndef _MZ_NET_PACKETSPECIFIC_HPP_
#define _MZ_NET_PACKETSPECIFIC_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <Metazion/Share/Memory/StepBuffer.hpp>

#include "Metazion/Net/PacketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class PacketSpecific {
    DISALLOW_COPY_AND_ASSIGN(PacketSpecific)
        
public:
    PacketSpecific(TransmitSocket& transmitSocket);
    
    ~PacketSpecific();
    
public:
    int Encode(int command, const void* data, int length, EncodeBuffer& encodeBuffer);
    
    int Decode(int& command, DecodeBuffer& decodeBuffer);

private:
    TransmitSocket& m_transmitSocket;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_PACKETSPECIFIC_HPP_
