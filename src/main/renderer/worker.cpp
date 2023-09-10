#include "worker.h"

amts::Worker::Worker() 
    : m_enabled(true),
        m_done(true), 
        m_workThread([this]() { working(); }), 
        m_job(nullptr) {

    std::cout << "Worker started !\n";
}

amts::Worker::~Worker() { 
    m_enabled = false;
    if (m_workThread.joinable()) {
        m_workThread.join();
    }

    std::cout << "Worker deleted !\n";
}

void amts::Worker::mark_as_done() {
    m_done = true;
}

amts::Job* amts::Worker::get_job() {
    return m_job;
}

void amts::Worker::submit_job(Job* job) {
    std::unique_lock<std::mutex> lock(mutex);
    m_done = false;
    m_job = job;
    lock.unlock();
    cv.notify_one();
}

void amts::Worker::disable() {
    m_enabled = false;
    m_workThread.join();
}

void amts::Worker::free_job() {
    std::unique_lock<std::mutex> lock(mutex);

    if(m_job != nullptr) {
        delete m_job;
        m_job = nullptr;
    }

    lock.unlock();
    cv.notify_one();
}

const bool& amts::Worker::is_done() const {
    return m_done;
}

const bool& amts::Worker::is_enabled() const {
    return m_enabled;
}

void amts::Worker::working() {
    while(m_enabled) {
        std::unique_lock<std::mutex> lock(mutex);

        cv.wait(lock, [this]() { 
            return m_job != nullptr || !m_enabled; 
        });

        if(!m_enabled)
            return;

        while(!m_job->is_done()) {
            m_job->do_job();

            if(m_job->is_done())
                m_done = true;
        }
    }
}
