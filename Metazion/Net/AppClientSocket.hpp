#ifndef MZ_NET_APPCLIENTSOCKET_HPP
#define MZ_NET_APPCLIENTSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#include "Metazion/Net/NormalClientSocket.hpp"
#include "Metazion/Net/PacketSpecific.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class AppClientSocket
    : public NormalClientSocket
    , public PacketProcessor {
    DISALLOW_COPY_AND_ASSIGN(AppClientSocket)

public:
    AppClientSocket();

    virtual ~AppClientSocket();

public:
    void Reset() override;

    void OnAttached() override;
    
    void OnStarted() override;

    int OnRecvData(const void* data, int length) override;

    bool SendData(int command, const void* data, int length);

    int PullPackets(void* buffer, int length, PacketArray_t& packetArray);

private:
    PackBuffer& GetPackBuffer();

private:
    PacketSpecific m_packetSpecific;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_APPCLIENTSOCKET_HPP
