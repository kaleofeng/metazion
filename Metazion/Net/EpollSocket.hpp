#ifndef MZ_NET_EPOLLSOCKET_HPP
#define MZ_NET_EPOLLSOCKET_HPP

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include "Metazion/Net/Socket.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class EpollSocket : public Socket {
    DISALLOW_COPY_AND_ASSIGN(EpollSocket)

    friend class EpollIoThread;
    friend class EpollMaintenanceThread;
    friend class EpollSocketServer;

public:
    EpollSocket();

    virtual ~EpollSocket();

protected:
    virtual void Input() = 0;

    virtual void Output() = 0;

    bool IsCanOutput() const {
        return m_canOutput;
    }

    void SetCanOutput(bool canOutput) {
        m_canOutput = canOutput;
    }

private:
    volatile bool m_canOutput;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_NET_EPOLLSOCKET_HPP
