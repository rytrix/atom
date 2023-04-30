#include "semaphore.hpp"
#include "fence.hpp"
#include "threadpool.hpp"

void test_semaphore(atom::pool& threadpool) {
    atom::semaphore semaphore(1); 
    atom::fence waitFence;
    auto waitDispatch = waitFence.dispatch();

    for (int i = 0; i < 10; i++) {
        threadpool.add_task([&waitDispatch,&semaphore,i](){
            auto dispatch = semaphore.wait(); 
            std::cout << "Print " << i << std::endl;
            if (i == 9) {
                waitDispatch.signal();
            }
        });
    }

    waitFence.wait();
}

void test_semaphore2(atom::pool& threadpool) {
    atom::semaphore semaphore(10);

    for(int i = 0; i < 10; i++) {
        threadpool.add_task([&semaphore,i](){
            auto dispatch = semaphore.wait(); 
            std::cout << "Print " << i << std::endl;
        });
    }
    
    semaphore.wait_complete();
    std::cout << "Threads working: " << threadpool.get_threads_working() << std::endl;
    //threadpool.wait_for_tasks();
    std::cout << "complete" << std::endl;
}

void test_fence(atom::pool& threadpool) {
    atom::fence fence;
    
    for (int i = 0; i < 10; i++) {
        threadpool.add_task([&fence,i](){
            auto dispatch = fence.dispatch(); 
            std::cout << "Print " << i << std::endl;
        });
    }
    
    threadpool.wait_for_tasks();
    fence.wait();

    std::cout << "Threads working: " << threadpool.get_threads_working() << std::endl;
    std::cout << "complete" << std::endl;
    //waitFence.wait();
}

//semaphore wait until signal is up to a certain count to print

int main() {

    atom::pool threadpool = atom::pool();

    //test_semaphore(threadpool);

    std::cout << "Semaphore2: " << std::endl;

    test_semaphore2(threadpool);

    std::cout << "Fence: " << std::endl;

    test_fence(threadpool);



    return 0;
}
