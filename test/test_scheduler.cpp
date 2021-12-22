#include "kvell.h"

static kvell::Logger::ptr g_logger = KV_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;
    KV_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    sleep(1);
    if(--s_count >= 0) {
        kvell::Scheduler::GetThis()->schedule(&test_fiber, kvell::GetThreadId());
    }
}

int main(int argc, char** argv) {
    KV_LOG_INFO(g_logger) << "main";
    kvell::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    KV_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    KV_LOG_INFO(g_logger) << "over";
    return 0;
}