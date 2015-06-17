#ifndef _MZ_NET_SELECTLISTENSTRATEGY_HPP_
#define _MZ_NET_SELECTLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class SelectListenStrategy final : public SelectStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectListenStrategy)

public:
    SelectListenStrategy(ListenSocket& m_listenSocket);

    ~SelectListenStrategy();

public:
    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    bool IsBusy() const override final;

    void PostInput() override final;

    void PostOutput() override final;

    void EnableOutput() override final;

    bool ShouldCareAboutOutput() const override final;

private:
    ListenSocket& m_listenSocket;
};

inline bool SelectListenStrategy::ShouldCareAboutOutput() const {
    return false;
}

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTLISTENSTRATEGY_HPP_
