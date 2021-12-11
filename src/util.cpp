#include "util.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

namespace kvell
{
    pid_t GetThreadId(){
        return syscall(SYS_gettid);
    }

    uint32_t GetFiberId(){
        return 0;
    }
} // namespace kvell
