#include "Metazion/Net/AppSocketServer.hpp"

#include <Metazion/Share/Thread/ThreadLocalStorage.hpp>

DECL_NAMESPACE_MZ_NET_BEGIN

AppSocketServer::AppSocketServer()
    : m_encodeBufferKey(0)
    , m_decodeBufferKey(0) {}

AppSocketServer::~AppSocketServer() {}

bool AppSocketServer::Initialize(int socketCapacity, int ioThreadNumber) {
    m_encodeBufferKey = NS_SHARE::ThreadLocalStorage::Alloc();
    if (m_encodeBufferKey == -1) {
        return false;
    }

    m_decodeBufferKey = NS_SHARE::ThreadLocalStorage::Alloc();
    if (m_decodeBufferKey == -1) {
        return false;
    }

    return NormalSocketServer::Initialize(socketCapacity, ioThreadNumber);
}

void AppSocketServer::Finalize() {
    NormalSocketServer::Finalize();

    if (m_decodeBufferKey != -1) {
        NS_SHARE::ThreadLocalStorage::Free(m_decodeBufferKey);
    }
    
    if (m_encodeBufferKey != -1) {
        NS_SHARE::ThreadLocalStorage::Free(m_encodeBufferKey);
    }
}

int AppSocketServer::LockSockets(SocketFilter& filter, SocketArray_t& socketArray) {
    socketArray.Clear();

    Lock();
    const auto socketCapacity = GetSocketCapacity();
    for (auto index = 0; index < socketCapacity; ++index) {
        auto socket = GetSocket(index);
        if (IsNull(socket)) {
            continue;
        }
        if (!filter.Filter(socket)) {
            continue;
        }

        socket->Retain();
        socketArray.Add(socket);
    }
    Unlock();

    return socketArray.GetSize();
}

void AppSocketServer::UnlockSockets(SocketArray_t& socketArray) {
    const auto size = socketArray.GetSize();
    for (auto index = 0; index < size; ++index) {
        auto socket = socketArray[index];
        ASSERT_TRUE(!IsNull(socket));
        socket->Release();
    }
}

EncodeBuffer& AppSocketServer::GetEncodeBuffer() {
    void* buffer = NS_SHARE::ThreadLocalStorage::GetValue(m_encodeBufferKey);
    if (IsNull(buffer)) {
        buffer = new ThreadEncodeBuffer();
        auto ret = NS_SHARE::ThreadLocalStorage::SetValue(m_encodeBufferKey, buffer);
        ASSERT_TRUE(ret);
    }

    auto threadBuffer = static_cast<ThreadEncodeBuffer*>(buffer);
    return threadBuffer->m_buffer;
}

DecodeBuffer& AppSocketServer::GetDecodeBuffer() {
    auto buffer = NS_SHARE::ThreadLocalStorage::GetValue(m_decodeBufferKey);
    if (IsNull(buffer)) {
        buffer = new ThreadDecodeBuffer();
        auto ret = NS_SHARE::ThreadLocalStorage::SetValue(m_decodeBufferKey, buffer);
        ASSERT_TRUE(ret);
    }

    auto threadBuffer = static_cast<ThreadDecodeBuffer*>(buffer);
    return threadBuffer->m_buffer;
}

DECL_NAMESPACE_MZ_NET_END
