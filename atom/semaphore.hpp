#pragma once

#ifndef _ATOM_SEMAPHORE_HPP
#define _ATOM_SEMAPHORE_HPP

#include <mutex>
#include <condition_variable>

namespace atom {

class semaphore {
    class handle;
    friend class handle;
public:
    semaphore(int value);
    ~semaphore();

    semaphore(const semaphore&) = delete;
    semaphore& operator=(const handle&) = delete;

    handle wait();
    handle wait(double seconds);
    void wait_complete();

private:
    const int m_default;
    int m_value;
    std::mutex m_mutex;
    std::condition_variable m_cond;

    class handle {
        friend class semaphore;
    public:
        ~handle();

        handle(const handle&) = delete;
        handle& operator=(const handle&) = delete;

        void post();

    private:
        handle(semaphore* sem);

        semaphore* sem;


    };

}; //class semaphore

} //namespace atom

#endif //_ATOM_SEMAPHORE_HPP
       


