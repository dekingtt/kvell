#ifndef __KVELL_LOG_H__
#define __KVELL_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <strstream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include "singleton.h"
#include "util.h"

#define KV_LOG_LEVEL(logger, level)                                                                                                     \
    if (logger->getLevel() <= level)                                                                                                    \
    kvell::LogEventWrap(kvell::LogEvent::ptr(new kvell::LogEvent(logger, level,                                                         \
                                                                 __FILE__, __LINE__, 0, kvell::GetThreadId(),                           \
                                                                 kvell::GetFiberId(), time(0), "UNKNOW" /*kvell::Thread::GetName()*/))) \
        .getSS()

#define KV_LOG_DEBUG(logger) KV_LOG_LEVEL(logger, kvell::LogLevel::DEBUG)
#define KV_LOG_INFO(logger) KV_LOG_LEVEL(logger, kvell::LogLevel::INFO)
#define KV_LOG_WARN(logger) KV_LOG_LEVEL(logger, kvell::LogLevel::WARN)
#define KV_LOG_ERROR(logger) KV_LOG_LEVEL(logger, kvell::LogLevel::ERROR)
#define KV_LOG_FATAL(logger) KV_LOG_LEVEL(logger, kvell::LogLevel::FATAL)

#define KV_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                                                       \
    if (logger->getLevel() <= level)                                                                                                    \
    kvell::LogEventWrap(kvell::LogEvent::ptr(new kvell::LogEvent(logger, level,                                                         \
                                                                 __FILE__, __LINE__, 0, kvell::GetThreadId(),                           \
                                                                 kvell::GetFiberId(), time(0), "UNKNOW" /*kvell::Thread::GetName()*/))) \
        .getEvent()                                                                                                                     \
        ->format(fmt, __VA_ARGS__)

#define KV_LOG_FMT_DEBUG(logger, fmt, ...) KV_LOG_FMT_LEVEL(logger, kvell::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define KV_LOG_FMT_INFO(logger, fmt, ...) KV_LOG_FMT_LEVEL(logger, kvell::LogLevel::INFO, fmt, __VA_ARGS__)
#define KV_LOG_FMT_WARN(logger, fmt, ...) KV_LOG_FMT_LEVEL(logger, kvell::LogLevel::WARN, fmt, __VA_ARGS__)
#define KV_LOG_FMT_ERROR(logger, fmt, ...) KV_LOG_FMT_LEVEL(logger, kvell::LogLevel::ERROR, fmt, __VA_ARGS__)
#define KV_LOG_FMT_FATAL(logger, fmt, ...) KV_LOG_FMT_LEVEL(logger, kvell::LogLevel::FATAl, fmt, __VA_ARGS__)

#define KV_LOG_ROOT() kvell::LoggerMgr::GetInstance()->getRoot()
#define KV_LOG_NAME(name) kvell::LoggerMgr::GetInstance()->getLogger(name)

namespace kvell
{
    class Logger;
    class LogFormatter;

    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };
        static std::string ToString(LogLevel::Level level);
        static LogLevel::Level FromString(const std::string &level);
    };
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
                 const char *file, uint32_t line, uint32_t elapse,
                 uint32_t threadId, uint32_t fiberId, uint32_t time,
                 const std::string &thread_name);
        const char *getFile() const { return m_file; }
        uint32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        const std::string &getThreadName() const { return m_threadName; }
        const std::string getContent() const { return m_content.str(); }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }
        LogLevel::Level getLevel() const { return m_level; }
        std::stringstream &getSS() { return m_content; };
        void format(const char *fmt, ... /*va_list al*/);
        void format(const char *fmt, va_list al);

    private:
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
        const char *m_file = nullptr;
        uint32_t m_line = 0;
        uint32_t m_elapse = 0;
        uint32_t m_threadId = 0;
        uint32_t m_fiberId = 0;
        uint64_t m_time = 0;
        std::stringstream m_content;
        std::string m_threadName;
    };

    class LogEventWrap
    {
    public:
        LogEventWrap(LogEvent::ptr e);
        ~LogEventWrap();
        LogEvent::ptr getEvent() const { return m_event; }
        std::stringstream &getSS();

    private:
        LogEvent::ptr m_event;
    };

    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string &pattern);

        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
        std::ostream &format(std::ostream &ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem
        {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){};
            virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        void init();
        bool isError() const { return m_error; }
        const std::string getPattern() const { return m_pattern; }

    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
        bool m_error = false;
    };

    class LogAppender
    {
        friend class Logger;

    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender(){};
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr formatter);
        LogFormatter::ptr getFormatter() { return m_formatter; };
        void setLevel(LogLevel::Level level) { m_level = level; };
        LogLevel::Level getLevel() const { return m_level; }

    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        bool m_hasFormatter = false;
        LogFormatter::ptr m_formatter;
    };

    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string &name);
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
        uint64_t m_lastTime = 0;
    };

    class Logger : public std::enable_shared_from_this<Logger>
    {
        friend class LoggerManager;

    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string &name = "root");
        void log(LogLevel::Level level, LogEvent::ptr event);
        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        void clearAppenders();
        LogLevel::Level getLevel() const { return m_level; };
        void setLevel(LogLevel::Level levle) { m_level = levle; };
        const std::string &getName() const { return m_name; }
        void setFormatter(LogFormatter::ptr formatter);
        void setFormatter(const std::string &val);
        LogFormatter::ptr getFormatter();

    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appenders;
        LogFormatter::ptr m_formatter;
        Logger::ptr m_root;
    };

    class LoggerManager
    {
    public:
        LoggerManager();
        Logger::ptr getLogger(const std::string &name);
        void init();
        Logger::ptr getRoot() const { return m_root; }

    private:
        std::map<std::string, Logger::ptr> m_loggers;
        Logger::ptr m_root;
    };
    typedef kvell::Singleton<LoggerManager> LoggerMgr;

}
#endif