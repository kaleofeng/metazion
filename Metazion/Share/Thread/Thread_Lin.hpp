#ifndef MZ_SHARE_THREAD_LIN_HPP
#define MZ_SHARE_THREAD_LIN_HPP

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

    int GetThreadId() const {
        return static_cast<int>(m_threadId);
    }
    
    void SetThreadId(int threadId) {
        m_threadId = static_cast<pthread_t>(threadId);
    }

protected:
    virtual void Execute() = 0;

private:
    static void* ThreadFunc(void* arg);

private:
    pthread_t m_threadId;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_LINUX

#endif // MZ_SHARE_THREAD_LIN_HPP
