#ifndef _MZ_SHARE_THREAD_LIN_HPP_
#define _MZ_SHARE_THREAD_LIN_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_LINUX)

#include <pthread.h>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class Thread {
    DISALLOW_COPY_AND_ASSIGN(Thread)

public:
    Thread();

    virtual ~Thread();

public:
    void Run();

    void Wait();

    int GetThreadId() const;
    
    void SetThreadId(int threadId);

protected:
    virtual void Execute() = 0;

private:
    static void* ThreadFunc(void* arg);

private:
    pthread_t m_threadId;
};

inline int Thread::GetThreadId() const {
    return static_cast<int>(m_threadId);
}

inline void Thread::SetThreadId(int threadId) {
    m_threadId = static_cast<pthread_t>(threadId);
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_LINUX

#endif // _MZ_SHARE_THREAD_LIN_HPP_
