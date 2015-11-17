#ifndef _MZ_SHARE_THREAD_HPP_
#define _MZ_SHARE_THREAD_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <thread>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(Thread)

private:
    std::thread m_thread;

public:
    Thread();

    virtual ~Thread();

public:
    void Run();

    void Join();

    void Detach();

    std::thread::id GetThreadId() const;

protected:
    virtual void Execute() = 0;

private:
    void ThreadFunc(); 
};

inline std::thread::id Thread::GetThreadId() const {
    return m_thread.get_id();
}

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_THREAD_HPP_
