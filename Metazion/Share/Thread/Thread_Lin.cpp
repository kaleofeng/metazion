#include "Metazion/Share/Thread/Thread_Lin.hpp"

#if defined(MZ_PLATFORM_LINUX)

DECL_NAMESPACE_MZ_SHARE_BEGIN

Thread::Thread()
    : m_threadId(0) {}

Thread::~Thread() {
    if (0 != m_threadId) {
        Wait();
    }
}

void Thread::Run() {
    const int ret = ::pthread_create(&m_threadId
        , nullptr
        , ThreadFunc
        , this);
    ASSERT_TRUE(0 == ret);
}

void Thread::Wait() {
    if (0 != m_threadId) {
        void* status = nullptr;
        const int ret = ::pthread_join(m_threadId, &status);
        ASSERT_TRUE(0 == ret);
        m_threadId = 0;
    }
}

void* Thread::ThreadFunc(void* arg) {
    auto thread = static_cast<Thread*>(arg);
    thread->Execute();
    return nullptr;
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_LINUX
