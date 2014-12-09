#ifndef _MZ_NET_IOCPIOTHREAD_HPP_
#define _MZ_NET_IOCPIOTHREAD_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_IOCP_MODEL)

#include <Metazion/Share/Thread/Thread.hpp>

#include "Metazion/Net/SocketCtrl.hpp"
#include "Metazion/Net/SocketDefine.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class NetworkService;

class IocpIoThread : public NS_SHARE::Thread {
    DISALLOW_COPY_AND_ASSIGN(IocpIoThread)

    enum Result {
        RESULT_SUCCESS = 1,
        RESULT_FAILURE,
        RESULT_CLOSE,
        RESULT_TIMEOUT,
        RESULT_ERROR,
    };

public:
    IocpIoThread();

    ~IocpIoThread();

public:
    void Initialize(NetworkService* networkService, int index);

    void Finalize();

protected:
    void Execute() override final;

private:
    int AnalyseStatusResult(BOOL ret
        , OVERLAPPED* overlapped
        , DWORD numberOfBytes
        , DWORD error);

private:
    volatile bool m_stopDesired;

    NetworkService* m_networkService;
    int m_index;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_IOCP_MODEL

#endif // _MZ_NET_IOCPIOTHREAD_HPP_
