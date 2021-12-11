#include <iostream>
#include <thread>
#include "../src/log.h"
//#include "src/util.h"

int main(int argc, char **argv)
{
    kvell::Logger::ptr logger(new kvell::Logger);
    logger->addAppender(kvell::LogAppender::ptr(new kvell::StdoutLogAppender));

    kvell::FileLogAppender::ptr file_appender(new kvell::FileLogAppender("./log.txt"));
    kvell::LogFormatter::ptr fmt(new kvell::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(kvell::LogLevel::ERROR);

    logger->addAppender(file_appender);

    //kvell::LogEvent::ptr event(new kvell::LogEvent(__FILE__, __LINE__, 0, kvell::GetThreadId(), kvell::GetFiberId(), time(0)));
    //event->getSS() << "hello kvell log";
    //logger->log(kvell::LogLevel::DEBUG, event);
    std::cout << "hello kvell log" << std::endl;

    KV_LOG_INFO(logger) << "test macro";
    KV_LOG_ERROR(logger) << "test macro error";

    KV_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = kvell::LoggerMgr::GetInstance()->getLogger("xx");
    KV_LOG_INFO(l) << "xxx";
    return 0;
}