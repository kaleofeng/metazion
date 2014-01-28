#ifndef _MZ_SHARE_THREAD_WIN_HPP_
#define _MZ_SHARE_THREAD_WIN_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#if defined(MZ_PLATFORM_WINOWS)

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

    int SetThreadId(int threadId);

protected:
    virtual void Execute() = 0;

private:
    static unsigned int __stdcall ThreadFunc(void* arg);

private:
    HANDLE m_hThread;
    DWORD m_threadId;
};

inline int Thread::GetThreadId() const {
    return static_cast<int>(m_threadId);
}

inline int Thread::SetThreadId(int threadId) {
    m_threadId = static_cast<DWORD>(threadId);
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_WINOWS

#endif // _MZ_SHARE_THREAD_WIN_HPP_
