#ifndef _ATOM_FENCE_CPP
#define _ATOM_FENCE_CPP

#include "fence.hpp"
#include <mutex>

namespace atom {

fence::fence() 
    :m_value(false)
{}

fence::~fence() = default;

fence::handle fence::dispatch() {
    std::unique_lock<std::mutex> lock(m_mutex);
    
    auto condition = [&](){return m_value == false;};
    m_cond.wait(lock,condition);
#ifdef _DEBUG
    if (m_value == true)
        throw std::runtime_error("Cannot dispatch multiple handles to the same fence");
#endif
    m_value = true;
    return {this};
}

void fence::wait() {
    std::unique_lock<std::mutex> lock(m_mutex);

    auto condition = [&](){return m_value == false;};

    m_cond.wait(lock,condition);
}

//handle
fence::handle::handle(fence* m_fence) 
    : m_fence(m_fence) 
{
#ifdef _DEBUG
    if (m_fence == nullptr)
        throw std::runtime_error("Cannot create a fence handle with a nullptr fence");
#endif
}

fence::handle::~handle() {
    signal();
}

void fence::handle::signal() {
    if (m_fence != nullptr)
    {
        std::lock_guard<std::mutex> lock(m_fence->m_mutex);
        m_fence->m_value = false;
        m_fence->m_cond.notify_all();
    }

    m_fence = nullptr;
}

} //namespace atom

#endif // _ATOM_FENCE_CPP

