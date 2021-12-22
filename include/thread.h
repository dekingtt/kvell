#ifndef __KVELL_THREAD_H__
#define __KVELL_THREAD_H__

#include <memory>
#include "mutex.h"
#include <string>
#include "noncopyable.h"

namespace kvell
{
    class Thread : Noncopyable
    {
    public:
        typedef std::shared_ptr<Thread> ptr;
        Thread(std::function<void()> cb, const std::string & name);

        ~Thread();
        pid_t getId() const {return m_id; }

        const std::string &getName() const { return m_name; }

        void join();

        static Thread *GetThis();

        static const std::string &GetName();

        static void SetName(const std::string &name);

    private:
        static void *run(void *arg);

    private:
        pid_t m_id = -1;
        pthread_t m_thread = 0;

        std::function<void()> m_cb;
        std::string m_name;
        Semaphore m_semaphore;
    };

} // namespace kvell

#endif
