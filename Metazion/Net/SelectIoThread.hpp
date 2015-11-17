#ifndef _MZ_NET_SELECTIOTHREAD_HPP_
#define _MZ_NET_SELECTIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include <atomic>

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class SelectIoThread : public NS_SHARE::Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectIoThread)

private:
    std::atomic<bool> m_stopDesired = { false };

    NetworkService* m_networkService = nullptr;
    int m_index = 0;
    int m_socketCount = 0;
    SocketCtrl* m_socketCtrlList = nullptr;
    fd_set m_rfds;
    fd_set m_wfds;
    fd_set m_efds;

public:
    SelectIoThread();

    ~SelectIoThread();

public:
    void Initialize(NetworkService* networkService, int index);

    void Finalize();

protected:
    void Execute() override final;

private:
    void ProcessEvents();

    int ResetFds();
};

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTIOTHREAD_HPP_
