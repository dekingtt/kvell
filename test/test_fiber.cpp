#include "kvell.h"

kvell::Logger::ptr g_logger = KV_LOG_ROOT();

void run_in_fiber() {
    KV_LOG_INFO(g_logger) << "run_in_fiber begin";
    kvell::Fiber::YieldToHold();
    KV_LOG_INFO(g_logger) << "run_in_fiber end";
    kvell::Fiber::YieldToHold();
}

void test_fiber() {
    KV_LOG_INFO(g_logger) << "main begin -1";
    {
        kvell::Fiber::GetThis();
        KV_LOG_INFO(g_logger) << "main begin";
        kvell::Fiber::ptr fiber(new kvell::Fiber(run_in_fiber));
        fiber->swapIn();
        KV_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        KV_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    KV_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv) {
    kvell::Thread::SetName("main");

    std::vector<kvell::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i) {
        thrs.push_back(kvell::Thread::ptr(
                    new kvell::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}