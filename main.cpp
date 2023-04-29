#include "semaphore.hpp"
#include "fence.hpp"
#include "threadpool.hpp"
#include <unistd.h>

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

void test_fence(atom::pool& threadpool) {
    atom::fence fence;
    atom::fence waitFence;

    auto waitDispatch = waitFence.dispatch();
    
    for (int i = 0; i < 10; i++) {
        threadpool.add_task([&waitDispatch,&fence,i](){
            auto dispatch = fence.dispatch(); 
            std::cout << "Print " << i << std::endl;
            if (i == 9) {
                waitDispatch.signal();
            }
        });
    }

    waitFence.wait();
}

//semaphore wait until signal is up to a certain count to print

int main() {

    //test print in order
    atom::pool threadpool = atom::pool();

    test_semaphore(threadpool);

    std::cout << "Fence: " << std::endl;

    test_fence(threadpool);



    return 0;
}
