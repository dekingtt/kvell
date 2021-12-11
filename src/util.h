#ifndef __KVELL_UTIL_H__
#define __KVELL_UTIL_H__

#include <thread>
#include <pthread.h>


namespace kvell
{
    pid_t GetThreadId();

    uint32_t GetFiberId();
} // namespace kvell


#endif