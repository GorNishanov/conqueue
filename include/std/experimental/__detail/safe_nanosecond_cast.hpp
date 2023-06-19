// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_DETAIL_SAFE_NANOSECOND_CAST
#define _STD_EXPERIMENTAL_DETAIL_SAFE_NANOSECOND_CAST

#include <chrono>

namespace std::experimental::__detail {

template <class _Rep, class _Period>
inline chrono::nanoseconds
safe_nanosecond_cast(chrono::duration<_Rep, _Period> __d) {
  using namespace chrono;
  using __ratio = ratio_divide<_Period, nano>;
  using __ns_rep = nanoseconds::rep;

  if constexpr (is_floating_point_v<_Rep>) {
    _Rep __result_float = __d.count() * __ratio::num / __ratio::den;

    _Rep __result_max = numeric_limits<__ns_rep>::max();
    if (__result_float >= __result_max) {
      return nanoseconds::max();
    }

    _Rep __result_min = numeric_limits<__ns_rep>::min();
    if (__result_float <= __result_min) {
      return nanoseconds::min();
    }

    return nanoseconds(static_cast<__ns_rep>(__result_float));
  } else {
    if (__d.count() == 0) {
      return nanoseconds(0);
    }
    __ns_rep __result_max = numeric_limits<__ns_rep>::max();
    if (__d.count() > 0 && __d.count() > __result_max / __ratio::num) {
      return nanoseconds::max();
    }

    __ns_rep __result_min = numeric_limits<__ns_rep>::min();
    if (__d.count() < 0 && __d.count() < __result_min / __ratio::num) {
      return nanoseconds::min();
    }

    __ns_rep __result = __d.count() * __ratio::num / __ratio::den;
    if (__result == 0) {
      return nanoseconds(1);
    }

    return nanoseconds(__result);
  }
}

} // namespace std::experimental::__detail

#endif // _STD_EXPERIMENTAL_DETAIL_SAFE_NANOSECOND_CAST