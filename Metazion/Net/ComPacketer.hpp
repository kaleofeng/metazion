#ifndef _MZ_NET_COMPACKETER_HPP_
#define _MZ_NET_COMPACKETER_HPP_

#include "Metazion/Net/NetInclude.hpp"

#include <functional>

#include "Metazion/Net/PacketCodec.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class TransmitSocket;

class ComPacketer {
    MZ_DISALLOW_COPY_AND_ASSIGN(ComPacketer)

    using ValidPacketCallback_t = std::function<void(int command, const void* data, int length)>;
    using InvalidPacketCallback_t = std::function<void()>;

public:
    ComPacketer(TransmitSocket& m_socket);

    ~ComPacketer();

public:
    void Reset();

    void Dispatch();

    bool SendData(int command, const void* data, int length);

    void PostData(int command, const void* data, int length);

    void SetValidPacketCallback(ValidPacketCallback_t callback);

    void SetInvalidPacketCallback(InvalidPacketCallback_t callback);

private:
    TransmitSocket& m_socket;

    PacketCodec m_packetCodec;

    ValidPacketCallback_t m_validPacketCallback;
    InvalidPacketCallback_t m_invalidPacketCallback;
};

inline void ComPacketer::SetValidPacketCallback(ValidPacketCallback_t callback) {
    m_validPacketCallback = callback;
}

inline void ComPacketer::SetInvalidPacketCallback(InvalidPacketCallback_t callback) {
    m_invalidPacketCallback = callback;
}

DECL_NAMESPACE_MZ_NET_END

#endif // _MZ_NET_COMPACKETER_HPP_
