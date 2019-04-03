#include "Metazion/Share/Async/AsyncService.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

AsyncService::AsyncService() {}

AsyncService::~AsyncService() {}

bool AsyncService::Initialize(int threadSize) {
    m_threadSize = threadSize;
    m_asyncThreads = new AsyncThread*[m_threadSize];
    for (auto index = 0; index < m_threadSize; ++index) {
        m_asyncThreads[index] = new AsyncThread(*this);
        m_asyncThreads[index]->Initialize();
    }

    return true;
}

void AsyncService::Finalize() {
    m_stopDesired = true;
    for (auto index = 0; index < m_threadSize; ++index) {
        m_asyncThreads[index]->WannaStop();
    }
    m_asyncCond.notify_all();

    for (auto index = 0; index < m_threadSize; ++index) {
        m_asyncThreads[index]->Finalize();
        SafeDelete(m_asyncThreads[index]);
    }
    SafeDeleteArray(m_asyncThreads);
    m_threadSize = 0;
}

void AsyncService::Push(Async_t async) {
    std::unique_lock<std::mutex> lock(m_asyncMutex);
    m_asyncQueue.push(async);
    m_asyncCond.notify_one();
}

AsyncService::Async_t AsyncService::Pop() {
    std::unique_lock<std::mutex> lock(m_asyncMutex);
    m_asyncCond.wait(lock, [this] {
        return !m_asyncQueue.empty() || m_stopDesired;
    });

    Async_t async{ nullptr };
    if (!m_asyncQueue.empty()) {
        async = m_asyncQueue.front();
        m_asyncQueue.pop();
    }
    return async;
}

DECL_NAMESPACE_MZ_SHARE_END
