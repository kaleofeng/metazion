#ifndef _MZ_SHARE_ASYNCSERVICE_HPP_
#define _MZ_SHARE_ASYNCSERVICE_HPP_

#include "Metazion/Share/ShareInclude.hpp"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

#include "Metazion/Share/Async/AsyncThread.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

class AsyncService {
    MZ_DISALLOW_COPY_AND_ASSIGN(AsyncService)

    using Async_t = std::function<void()>;
    using AsyncQueue_t = std::queue<Async_t>;

private:
    AsyncThread** m_asyncThreads{ nullptr };
    int m_threadSize{ 0 };
    std::atomic<bool> m_stopDesired{ false };

    std::mutex m_asyncMutex;
    std::condition_variable m_asyncCond;
    AsyncQueue_t m_asyncQueue;

public:
    AsyncService();

    ~AsyncService();

    bool Initialize(int threadSize);

    void Finalize();

    void Push(Async_t async);

    Async_t Pop();
};

DECL_NAMESPACE_MZ_SHARE_END

#endif // _MZ_SHARE_ASYNCSERVICE_HPP_
