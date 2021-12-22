#include "kvell.h"
#include <unistd.h>

kvell::Logger::ptr g_logger = KV_LOG_ROOT();

int count = 0;
//kvell::RWMutex s_mutex;
kvell::Mutex s_mutex;

void fun1() {
    KV_LOG_INFO(g_logger) << "name: " << kvell::Thread::GetName()
                             << " this.name: " << kvell::Thread::GetThis()->getName()
                             << " id: " << kvell::GetThreadId()
                             << " this.id: " << kvell::Thread::GetThis()->getId();

    for(int i = 0; i < 100000; ++i) {
        //kvell::RWMutex::WriteLock lock(s_mutex);
        kvell::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        KV_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        KV_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    KV_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/sylar/test/sylar/bin/conf/log2.yml");
    kvell::Config::LoadFromYaml(root);

    std::vector<kvell::Thread::ptr> thrs;
    for(int i = 0; i < 1; ++i) {
        kvell::Thread::ptr thr(new kvell::Thread(&fun2, "name_" + std::to_string(i * 2)));
        //kvell::Thread::ptr thr2(new kvell::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        //thrs.push_back(thr2);
    }

    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    KV_LOG_INFO(g_logger) << "thread test end";
    KV_LOG_INFO(g_logger) << "count=" << count;

    return 0;
}