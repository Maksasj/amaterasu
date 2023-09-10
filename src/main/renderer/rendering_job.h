#ifndef _RENDERING_JOB_H_
#define _RENDERING_JOB_H_

#include "job.h"

namespace amts {
    struct RenderWorkArea {
        Vec2<u64> m_start;
        Vec2<u64> m_size; 
    };

    class RenderingJob : public Job {
        private:
            const RenderWorkArea m_workArea;
            std::function<void(RenderingJob*)> m_renderingLambda;

        public:
            RenderingJob(const RenderWorkArea& workArea, const std::function<void(RenderingJob*)>& renderingLambda) 
                : Job(), 
                  m_workArea(workArea),
                  m_renderingLambda(renderingLambda) {
                
            }

            const RenderWorkArea& get_work_area() const {
                return m_workArea;
            }

            void do_job() override {
                m_renderingLambda(this);
            }
    };
}

#endif