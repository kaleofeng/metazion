#include "Metazion/Share/Thread/Thread_Win.hpp"

#if defined(MZ_PLATFORM_WINOWS)

#include <process.h>

DECL_NAMESPACE_MZ_SHARE_BEGIN

Thread::Thread()
    : m_hThread(0)
    , m_threadId(0) {}

Thread::~Thread() {
    if (0 != m_threadId) {
        Wait();
    }
}

void Thread::Run() {
    unsigned int threadId = 0;
    uintptr_t ret = ::_beginthreadex(nullptr
        , 0
        , ThreadFunc
        , this
        , 0
        , &threadId);
    m_hThread = reinterpret_cast<HANDLE>(ret);
    m_threadId = threadId;
}

void Thread::Wait() {
    if (0 != m_threadId) {
        ::WaitForSingleObject(m_hThread, INFINITE);
        ::CloseHandle(m_hThread);
        m_threadId = 0;
    }
}

unsigned int __stdcall Thread::ThreadFunc(void* arg) {
    auto thread = static_cast<Thread*>(arg);
    thread->Execute();
    return 0;
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_WINOWS
