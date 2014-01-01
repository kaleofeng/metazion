#ifndef MZ_SHARE_THREAD_WIN_HPP
#define MZ_SHARE_THREAD_WIN_HPP

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

    int GetThreadId() const {
        return static_cast<int>(m_threadId);
    }

    int SetThreadId(int threadId) {
        m_threadId = static_cast<DWORD>(threadId);
    }

protected:
    virtual void Execute() = 0;

private:
    static unsigned int __stdcall ThreadFunc(void* arg);

private:
    HANDLE m_hThread;
    DWORD m_threadId;
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // MZ_PLATFORM_WINOWS

#endif // MZ_SHARE_THREAD_WIN_HPP
