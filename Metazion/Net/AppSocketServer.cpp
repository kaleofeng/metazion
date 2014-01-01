#include "Metazion/Net/AppSocketServer.hpp"

#include <Metazion/Share/Thread/ThreadLocalStorage.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

AppSocketServer::AppSocketServer()
    : m_packBufferKey(0) {}

AppSocketServer::~AppSocketServer() {}

bool AppSocketServer::Initialize(int socketCapacity, int ioThreadNumber) {
    m_packBufferKey = NS_SHARE::ThreadLocalStorage::Alloc();
    if (-1 == m_packBufferKey) {
        return false;
    }

    return NormalSocketServer::Initialize(socketCapacity, ioThreadNumber);
}

void AppSocketServer::Finalize() {
    NormalSocketServer::Finalize();
    
    if (-1 != m_packBufferKey) {
        NS_SHARE::ThreadLocalStorage::Free(m_packBufferKey);
    }
}

int AppSocketServer::LockSockets(SocketFilter& filter, SocketArray_t& socketArray) {
    socketArray.Clear();

    Lock();
    const int socketCapacity = GetSocketCapacity();
    for (int index = 0; index < socketCapacity; ++index) {
        Socket* socket = GetSocket(index);
        if (IsNull(socket)) {
            continue;
        }
        if (!filter.Filter(socket)) {
            continue;
        }

        socket->GrabRef();
        socketArray.Add(socket);
    }
    Unlock();

    return socketArray.GetSize();
}

void AppSocketServer::UnlockSockets(SocketArray_t& socketArray) {
    const int size = socketArray.GetSize();
    for (int index = 0; index < size; ++index) {
        Socket* socket = socketArray[index];
        ASSERT_TRUE(!IsNull(socket));
        socket->ReleaseRef();
    }
}

PackBuffer& AppSocketServer::GetPackBuffer() {
    void* buffer = NS_SHARE::ThreadLocalStorage::GetValue(m_packBufferKey);
    if (IsNull(buffer)) {
        buffer = new ThreadPackBuffer();
        bool ret = NS_SHARE::ThreadLocalStorage::SetValue(m_packBufferKey, buffer);
        ASSERT_TRUE(ret);
    }

    ThreadPackBuffer* threadBuffer = static_cast<ThreadPackBuffer*>(buffer);
    return threadBuffer->m_packBuffer;
}

DECL_NAMESPACE_MZ_NET_END
