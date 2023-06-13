// Copyright (c) 2023 Gor Nishanov
// Licenced under MIT license. See LICENSE.txt for details.

#include "std/experimental/conqueue"
#include <system_error>

namespace std::experimental {

static const char* get_string(conqueue_errc errc) {
  switch (errc) {
  case conqueue_errc::success:
    return "success";
  case conqueue_errc::empty:
    return "queue is empty";
  case conqueue_errc::full:
    return "queue is full";
  case conqueue_errc::closed:
    return "queue is closed";
  default:
    return "invalid conqueue_errc value";
  }
}

struct __conqueue_error_category : std::error_category {
public:
  const char* name() const noexcept override { return "conqueue"; };
  string message(int ev) const override {
    return get_string(static_cast<conqueue_errc>(ev));
  };
};

const error_category& conqueue_category() noexcept {
  static __conqueue_error_category cat;
  return cat;
}

conqueue_error::conqueue_error(const std::error_code& ec)
    : runtime_error(get_string(static_cast<conqueue_errc>(ec.value()))),
      ec(ec) {}

conqueue_error::~conqueue_error() noexcept {}

} // namespace std::experimental