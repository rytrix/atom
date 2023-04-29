#ifndef _ATOM_POOL_CPP
#define _ATOM_POOL_CPP

#include "threadpool.hpp"

namespace atom {

pool::pool() {
    spawn_threads(std::thread::hardware_concurrency());
}

pool::pool(uint32_t thread_count) {
    spawn_threads(thread_count);
}

pool::~pool() {
    join_threads();
}

void pool::spawn_threads(uint32_t count) {
    _stop_requested = false;
    size_t original_size = threads.size();
    threads.resize(original_size+count);
#ifdef _DEBUG
    std::cout << "Thread count: " << threads.size() << std::endl;
#endif
    for (size_t i = original_size; i < threads.size(); i++) {
        threads[i] = std::thread(&pool::wait_for_task, this);
    }
}

void pool::join_threads() {
    while (has_tasks() && _stop_requested == false) {
        std::this_thread::yield();
    }
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        _stop_requested = true;
        // queue.push(nullptr);
        queue_cond.notify_all();
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

void pool::add_task(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    queue.push(task);
    queue_cond.notify_one();
}

void pool::wait_for_tasks() {
    while (has_tasks()) {
        std::this_thread::yield();
    }
}

bool pool::has_tasks() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return !queue.empty();
}

void pool::empty() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    while (!queue.empty()) {
        queue.pop();
    }
}

void pool::wait_for_task() {
    while (true) {
        std::function<void()> task = nullptr;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cond.wait(lock, [this] { return _stop_requested || !queue.empty(); });
            if (_stop_requested) {
                return;
            }
            task = queue.front();
            queue.pop();
        }
        if (task) {
            task();
        }
    }
}

} //namespace atom

#endif //_ATOM_POOL_CPP


