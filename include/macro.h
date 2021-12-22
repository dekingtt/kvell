#ifndef __KVELL_MACRO_H__
#define __KVELL_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define KVELL_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define KVELL_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define KVELL_LIKELY(x)      (x)
#   define KVELL_UNLIKELY(x)      (x)
#endif

#define KVELL_ASSERT(x) \
    if(KVELL_UNLIKELY(!(x))) { \
        KV_LOG_ERROR(KV_LOG_ROOT()) << "ASSERtION: " #x \
            << "\nbacktrace: \n" \
            << kvell::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define KVELL_ASSERT2(x, w) \
    if(KVELL_UNLIKELY(!(x))) { \
        KV_LOG_ERROR(KV_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << kvell::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }


#endif