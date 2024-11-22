// Copyright (c) 2023 Gor Nishanov
// Licensed under MIT license. See LICENSE.txt for details.

#ifndef _STD_EXPERIMENTAL_CONQUEUE_TRACING
#define _STD_EXPERIMENTAL_CONQUEUE_TRACING

#ifndef STDEX_CONQUEUE_ENABLE_TRACING
#define STDEX_CONQUEUE_LOG(...)
#else
#include <stdio.h>
#define STDEX_CONQUEUE_LOG(...) printf(__VA_ARGS__)
#endif

#endif // _STD_EXPERIMENTAL_CONQUEUE_TRACING
