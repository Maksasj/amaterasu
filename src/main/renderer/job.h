#ifndef _JOB_H_
#define _JOB_H_

#include "common/common.h"

namespace amts {
    class Job {
        private:
            bool m_done;
            f32 m_completion;

        public:
            Job() 
                : m_done(false), 
                  m_completion(0) {

            }

            virtual void do_job() = 0;

            const f32& get_completion_rate() const {
                return m_completion;
            }

            void mark_done() {
                m_done = true;
            }

            const bool& is_done() const {
                return m_done;
            }
    };
}

#endif