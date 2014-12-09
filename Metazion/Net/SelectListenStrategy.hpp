#ifndef _MZ_NET_SELECTLISTENSTRATEGY_HPP_
#define _MZ_NET_SELECTLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class SelectListenStrategy final : public SelectStrategy {
    DISALLOW_COPY_AND_ASSIGN(SelectListenStrategy)

public:
    SelectListenStrategy(ListenSocket& m_listenSocket);

    ~SelectListenStrategy();

public:
    void Reset() override;

    void Prepare() override;

    void Tick(int interval) override;

    void Start() override;

    bool IsBusy() const override;

    void DoInput() override;

    void DoOutput() override;

    void EnableOutput() override;

private:
    ListenSocket& m_listenSocket;
};

DECL_NAMESPACE_MZ_NET_END

#endif // NETWORK_USE_SELECT_MODEL

#endif // _MZ_NET_SELECTLISTENSTRATEGY_HPP_
