#ifndef _MZ_NET_APPSERVERSOCKET_HPP_
#define _MZ_NET_APPSERVERSOCKET_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalServerSocket.hpp"
#include "Metazion/Net/PacketSpecific.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppServerSocket 
    : public NormalServerSocket
    , public PacketProcessor {
    DISALLOW_COPY_AND_ASSIGN(AppServerSocket)

public:
    AppServerSocket();

    virtual ~AppServerSocket();

public:
    void Reset() override;

    void OnAttached() override;

    void OnStarted() override;

    int OnRecvData(const void* data, int length) override;

    int PullPackets(void* buffer, int length, PacketArray_t& packetArray) override;

    bool SendData(int command, const void* data, int length);

private:
    PackBuffer& GetPackBuffer();

private:
    PacketSpecific m_packetSpecific;
};

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_APPSERVERSOCKET_HPP_
