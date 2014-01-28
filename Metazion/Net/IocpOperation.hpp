#ifndef _MZ_NET_IOCPOPERATION_HPP_
#define _MZ_NET_IOCPOPERATION_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

struct IocpOperation {
    enum Type {
        TYPE_ACCEPT = 1,
        TYPE_SEND,
        TYPE_RECV,
    };

    IocpOperation(Type type)
        : m_type(type)
        , m_busy(false) { ::memset(&m_overlapped, 0, sizeof(m_overlapped)); }

    void Reset() {
        ::memset(&m_overlapped, 0, sizeof(m_overlapped));
        m_busy = false;
    }

    bool IsBusy() const {
        return m_busy;
    }

    void SetBusy(bool busy) {
        m_busy = busy;
    }

    Type m_type;
    OVERLAPPED m_overlapped;
    volatile bool m_busy;
};

struct AcceptOperation final : public IocpOperation {
    AcceptOperation()
        : IocpOperation(TYPE_ACCEPT)
        , m_buffer(nullptr)
        , m_sockId(INVALID_SOCKID) {}

    void Reset() {
        IocpOperation::Reset();
        m_buffer = nullptr;
        m_sockId = INVALID_SOCKID;
    }

    char* m_buffer;
    SockId_t m_sockId;
};

struct SendOperation final : public IocpOperation {
    SendOperation()
        : IocpOperation(TYPE_SEND) {}
    
    void Reset() {
        IocpOperation::Reset();
        ::memset(&m_wsaBuf, 0, sizeof(m_wsaBuf));
    }

    WSABUF m_wsaBuf;
};

struct RecvOperation final : public IocpOperation {
    RecvOperation()
        : IocpOperation(TYPE_RECV) {}

    void Reset() {
        IocpOperation::Reset();
        ::memset(&m_wsaBuf, 0, sizeof(m_wsaBuf));
    }

    WSABUF m_wsaBuf;
};

DECL_NAMESPACE_MZ_NET_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_NET_IOCPOPERATION_HPP_
