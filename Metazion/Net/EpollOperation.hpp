#ifndef _MZ_NET_EPOLLOPERATION_HPP_
#define _MZ_NET_EPOLLOPERATION_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_EPOLL_MODEL)

#include <atomic>

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

struct EpollOperation {
    enum Type {
        TYPE_ACCEPT = 1,
        TYPE_SEND,
        TYPE_RECV,
    };

    Type m_type;
    std::atomic<bool> m_busy{ false };

    EpollOperation(Type type)
        : m_type(type) {}

    void Reset() {
        m_busy = false;
    }

    bool IsBusy() const {
        return m_busy;
    }

    void SetBusy(bool busy) {
        m_busy = busy;
    }
};

struct AcceptOperation final : public EpollOperation {
    char* m_buffer{ nullptr };
    SockId_t m_sockId = INVALID_SOCKID;
    
    AcceptOperation()
        : EpollOperation(TYPE_ACCEPT) {}

    void Reset() {
        EpollOperation::Reset();
        m_buffer = nullptr;
        m_sockId = INVALID_SOCKID;
    }
};

struct SendOperation final : public EpollOperation {
    IOV_TYPE m_iov[NUMBER_SEND_IOV];

    SendOperation()
        : EpollOperation(TYPE_SEND) {}
    
    void Reset() {
        EpollOperation::Reset();
        memset(&m_iov, 0, sizeof(m_iov));
    }
};

struct RecvOperation final : public EpollOperation {
    IOV_TYPE m_iov[NUMBER_RECV_IOV];

    RecvOperation()
        : EpollOperation(TYPE_RECV) {}

    void Reset() {
        EpollOperation::Reset();
        memset(&m_iov, 0, sizeof(m_iov));
    }
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_EPOLLOPERATION_HPP_
