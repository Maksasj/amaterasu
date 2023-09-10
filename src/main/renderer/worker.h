#ifndef _WORKER_H_
#define _WORKER_H_

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

#include "job.h"

namespace amts {
    class Worker {
        private:
            bool m_enabled;
            bool m_done;

            std::thread m_workThread;
            Job* m_job;

        public:

            std::condition_variable cv;
            std::mutex mutex;

            Worker();
            ~Worker();

            void working();

            void mark_as_done();

            Job* get_job();

            void free_job();

            void submit_job(Job* job);

            void disable();

            const bool& is_done() const;

            const bool& is_enabled() const;
    };
}

#endif