#pragma once

#ifndef _ATOM_FENCE_HPP
#define _ATOM_FENCE_HPP

#include <mutex>
#include <condition_variable>

namespace atom {

class fence {
class handle;
public:
    fence();
    ~fence();

    fence(const fence&) = delete;
    fence& operator=(const fence&) = delete;
    
    handle dispatch();
    void wait();

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_value;

    class handle {
        friend class fence;
    public:
        ~handle();
        void signal();

        handle(const handle&) = delete;
        handle& operator=(const handle&) = delete;

    private:
        handle(fence* m_fence);
        fence* m_fence = nullptr;

    };

}; //class fence

} //namespace atom

#endif //_ATOM_FENCE_HPP

