#ifndef MZ_NET_APPSERVERSOCKET_HPP
#define MZ_NET_APPSERVERSOCKET_HPP

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

public: // @override
    void Reset();

    void OnAttached();

    void OnStarted();

    int OnRecvData(const void* data, int length);

public:
    bool SendData(int command, const void* data, int length);

    bool SendDataNow(int command, const void* data, int length);

    int PullPackets(void* buffer, int length, PacketArray_t& packetArray);

private:
    PackBuffer& GetPackBuffer();

private:
    PacketSpecific m_packetSpecific;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_NET_APPSERVERSOCKET_HPP
