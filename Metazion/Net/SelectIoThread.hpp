#ifndef _MZ_NET_SELECTIOTHREAD_HPP_
#define _MZ_NET_SELECTIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class SelectIoThread : public NS_SHARE::Thread {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectIoThread)

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

private:
    volatile bool m_stopDesired;
    
    NetworkService* m_networkService;
    int m_index;
    int m_socketCount;
    SocketCtrl* m_socketCtrlList;
    fd_set m_rfds;
    fd_set m_wfds;
    fd_set m_efds;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_SELECT_MODEL

#endif // _MZ_NET_SELECTIOTHREAD_HPP_
