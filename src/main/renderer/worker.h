#ifndef _WORKER_H_
#define _WORKER_H_

#include <thread>
#include <functional>

namespace amts {
    class Job {
        private:
            bool m_done;

        public:
            Job() : m_done(false) {

            }

            virtual void do_job() = 0;

            const bool& is_done() const {
                return m_done;
            }
    };

    class Worker {
        private:
            bool m_enabled;
            bool m_done;

            std::thread m_workThread;
            Job* m_job;

        public:
            Worker() 
                : m_enabled(true),
                  m_done(false), 
                  m_workThread(working, this), 
                  m_job(nullptr) {

            }

            void working(Worker* worker) {
                while((m_job != nullptr) && !m_job->is_done()) {
                    m_job->do_job();

                    if(m_job->is_done())
                        m_done = true;
                }
            }

            void submit_job(Job* job) {
                m_done = false;
                m_job = job;
            }

            void disable() {
                m_enabled = false;
                m_workThread.join();
            }

            const bool& is_done() const {
                return m_done;
            }
    };
}

#endif