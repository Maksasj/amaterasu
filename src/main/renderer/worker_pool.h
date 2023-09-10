#ifndef _WORKER_POOL_H_
#define _WORKER_POOL_H_

#include <vector>
#include "worker.h"

namespace amts {
    struct WorkerPool {
        std::vector<std::unique_ptr<Worker>> m_workers;

        WorkerPool() {
            const u32 workerThreadCount = 8;
            // const u32 workerThreadCount = std::thread::hardware_concurrency();

            for(u32 i = 0; i < workerThreadCount; ++i)
                m_workers.emplace_back(std::make_unique<Worker>());

            std::cout << "Created " << workerThreadCount << " workers\n";
        }

        bool all_workers_done() const {
            for(const auto& worker : m_workers) {
                if(!worker->is_done())
                    return false;
            }

            return true;
        }

        Worker* get_unbusy_worker() {
            for(auto& worker : m_workers) {
                if(worker->is_done())
                    return worker.get();
            }

            return nullptr; 
        }
    };
}

#endif