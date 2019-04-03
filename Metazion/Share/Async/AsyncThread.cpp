#include "Metazion/Share/Async/AsyncThread.hpp"

#include "Metazion/Share/Async/AsyncService.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

AsyncThread::AsyncThread(AsyncService& asyncService)
    : m_asyncService(asyncService) {}

AsyncThread::~AsyncThread() {}

bool AsyncThread::Initialize() {
    m_thread = std::thread(&AsyncThread::ThreadFunc, this);
    m_stopDesired = false;
    return true;
}

void AsyncThread::Finalize() {
    m_stopDesired = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void AsyncThread::WannaStop() {
    m_stopDesired = true;
}

void AsyncThread::ThreadFunc() {
    while (!m_stopDesired) {
        const auto async = m_asyncService.Pop();
        if (async != nullptr) {
            async();
        }
    }
}

DECL_NAMESPACE_MZ_SHARE_END
