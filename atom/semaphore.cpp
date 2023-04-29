#ifndef _ATOM_SEMAPHORE_CPP
#define _ATOM_SEMAPHORE_CPP

#include "semaphore.hpp"

namespace atom {

semaphore::semaphore(int value)
    : m_value(value)
{}

semaphore::~semaphore() = default;

semaphore::handle semaphore::wait() {
    auto condition = [&](){return m_value != 0;};

    std::unique_lock<std::mutex> lock(m_mutex);

    m_cond.wait(lock,condition);

    m_value--;

    return {this};
}

semaphore::handle semaphore::wait(double seconds) {
    std::chrono::duration<double> secs(seconds);
    auto timeout = std::chrono::duration_cast<std::chrono::milliseconds>(secs);

    auto condition = [&](){return m_value != 0;};
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_cond.wait_for(lock,timeout,condition) == false) {
        return {nullptr};
    }

    m_value--;

    return {this};
}
//class handle

semaphore::handle::handle(semaphore* sem) 
    : sem(sem)
{
   if (sem == nullptr)
       throw std::runtime_error("A nullptr semaphore cannot create a handle");
}

semaphore::handle::~handle() {
    post();
}

void semaphore::handle::post() {
    if (sem == nullptr)
        throw std::runtime_error("A handle cannot post the same semaphore multiple times");
    {
        std::lock_guard<std::mutex> lock(sem->m_mutex);
        sem->m_value++;
    }
    sem->m_cond.notify_all();
    sem = nullptr;
}

} //namespace atom

#endif



