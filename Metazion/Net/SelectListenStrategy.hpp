#ifndef _MZ_NET_SELECTLISTENSTRATEGY_HPP_
#define _MZ_NET_SELECTLISTENSTRATEGY_HPP_

#include "Metazion/Net/NetInclude.hpp"

#if defined(NETWORK_USE_SELECT_MODEL)

#include "Metazion/Net/SelectStrategy.hpp"

DECL_NAMESPACE_MZ_NET_BEGIN

class ListenSocket;

class SelectListenStrategy final : public SelectStrategy {
    MZ_DISALLOW_COPY_AND_ASSIGN(SelectListenStrategy)

private:
    ListenSocket& m_listenSocket;
    std::atomic<bool> m_canInput{ false };
    std::atomic<bool> m_canOutput{ false };

public:
    SelectListenStrategy(ListenSocket& m_listenSocket);

    ~SelectListenStrategy();

    void Reset() override final;

    void Prepare() override final;

    void Start() override final;

    void Launch() override final;

    bool IsBusy() const override final;

    void PostInput() override final;

    void PostOutput() override final;

    void EnableInput() override final;

    void EnableOutput() override final;

    bool ShouldCareAboutOutput() const override final;
};

inline bool SelectListenStrategy::ShouldCareAboutOutput() const {
    return false;
}

DECL_NAMESPACE_MZ_NET_END

#endif

#endif // _MZ_NET_SELECTLISTENSTRATEGY_HPP_
