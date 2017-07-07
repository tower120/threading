#ifndef THREADING_TEST_TESTRWSPINLOCK_H
#define THREADING_TEST_TESTRWSPINLOCK_H

#include <thread>
#include <chrono>
#include <shared_mutex>
#include <threading/RWSpinLock.h>

class TestRWSpinLock{
public:
    void test_1(){
        const int count = 10;
        using namespace threading;
        using Lock = RWSpinLock;
        RWSpinLock lock;

        int counter = 0;

        std::thread t1([&](){
            for(int i = 0; i<count;i++) {
                std::shared_lock<Lock> l(lock);
                std::cout << "t1 " << counter << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds(500) );
            }
        });
        std::thread t2([&](){

            for(int i = 0; i<count;i++) {
                std::shared_lock<Lock> l(lock);
                std::cout << "t2 " << counter << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds(500) );
            }
        });

        std::thread t3([&](){


            for(int i = 0; i<count;i++) {
                std::unique_lock<Lock> l(lock);
                counter = i;
                std::this_thread::sleep_for( std::chrono::milliseconds(500) );
            }
        });

        t1.join();
        t2.join();
        t3.join();
    }

    void test_shared_lock(){
        using namespace threading;
        using Lock = RWSpinLock;
        RWSpinLock lock;

        int counter = 0;
        std::thread t1([&](){
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "t1 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "t1 end" << counter << std::endl;
            }
        });
        std::thread t2([&](){
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "t2 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                std::cout << "t2 end" << counter << std::endl;
            }
        });

        std::thread t3([&](){
            {
                std::unique_lock<Lock> l(lock);
                std::cout << "unique_lock begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "unique_lock end" << counter << std::endl;
            }
        });

        t1.join();
        t2.join();
        t3.join();
    }

    void test_all(){
        //test_1();
        test_shared_lock();
    }
};

#endif //THREADING_TEST_TESTRWSPINLOCK_H
