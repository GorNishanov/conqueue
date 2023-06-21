// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_CONQUEUE_EASY_CANCEL
#define _STD_EXPERIMENTAL_CONQUEUE_EASY_CANCEL

#include <cassert>
#include <stdexec/execution.hpp>
#include <variant>

namespace std::experimental::__detail {

template <typename Receiver, typename CancelCallback, typename StopToken,
          bool Unstoppable>
struct easy_cancel_base;

template <typename Receiver, typename CancelCallback, typename StopToken>
struct easy_cancel_base<Receiver, CancelCallback, StopToken, false> {
  using StopCallback =
      typename StopToken::template callback_type<CancelCallback>;

  variant<monostate, StopToken, StopCallback> state;

  easy_cancel_base(Receiver& receiver)
      : state(stdexec::get_stop_token(stdexec::get_env(receiver))) {}

  bool stop_requested() {
    if (auto* tok = std::get_if<StopToken>(&state))
      return tok->stop_requested();

    return false;
  }

  void emplace(CancelCallback&& cancel_callback) {
    auto token = std::move(std::get<StopToken>(state));
    state.template emplace<2>(std::move(token), std::move(cancel_callback));
  }

  void reset() { state.template emplace<0>(); }
};

template <typename Receiver, typename CancelCallback, typename StopToken>
struct easy_cancel_base<Receiver, CancelCallback, StopToken, true> {
  constexpr easy_cancel_base(Receiver&) {}

  constexpr bool stop_requested() { return false; }

  template <typename F> void register_callback(F&&) {}

  void reset() {}
};

template <typename Receiver, typename CancelCallback,
          typename StopToken =
              stdexec::stop_token_of_t<stdexec::env_of_t<Receiver>>>
struct easy_cancel : easy_cancel_base<Receiver, CancelCallback, StopToken,
                                      stdexec::unstoppable_token<StopToken>> {
  using base = easy_cancel_base<Receiver, CancelCallback, StopToken,
                                stdexec::unstoppable_token<StopToken>>;

  using base::base;
};

} // namespace std::experimental::__detail

#endif // _STD_EXPERIMENTAL_CONQUEUE_EASY_CANCEL
