#ifndef _MZ_SHARE_ASYNCTHREAD_HPP_
#define _MZ_SHARE_ASYNCTHREAD_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <atomic>
#include <thread>

DECL_NAMESPACE_MZ_SHARE_BEGIN

class AsyncService;

class AsyncThread {
    MZ_DISALLOW_COPY_AND_ASSIGN(AsyncThread)

private:
    AsyncService& m_asyncService;
    std::thread m_thread;
    std::atomic<bool> m_stopDesired{ false };

public:
    AsyncThread(AsyncService& asyncService);

    ~AsyncThread();

    bool Initialize();

    void Finalize();

private:
    void ThreadFunc();
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_ASYNCTHREAD_HPP_
