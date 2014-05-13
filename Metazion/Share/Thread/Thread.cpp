#include "Metazion/Share/Thread/Thread.hpp"

DECL_NAMESPACE_MZ_SHARE_BEGIN

Thread::Thread() {}

Thread::~Thread() {}

void Thread::Run() {
    m_thread = std::thread(std::bind(&Thread::ThreadFunc, this));
}

void Thread::Join() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void Thread::Detach() {
    m_thread.detach();
}

void Thread::ThreadFunc() {
    Execute();
}

DECL_NAMESPACE_MZ_SHARE_END