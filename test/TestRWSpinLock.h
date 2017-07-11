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
        using Lock = RWSpinLockReaderBiased<>;
        Lock lock;

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
        //using Lock = RWSpinLockReaderBiased<>;
        using Lock = RWSpinLockWriterBiased<>;
        Lock lock;

        int counter = 0;
        std::thread t0([&](){
            std::cout << "t0" << std::endl;
            {
                std::unique_lock<Lock> l(lock);
                counter++;
                std::cout << "unique_lock0 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "unique_lock0 end" << counter << std::endl;
            }
        });

        std::thread t1([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "t1" << std::endl;
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "t1 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "t1 end" << counter << std::endl;
            }
        });
        std::thread t2([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "t2" << std::endl;
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "t2 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                std::cout << "t2 end" << counter << std::endl;
            }
        });

        std::thread t3([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            std::cout << "t3" << std::endl;
            {
                std::unique_lock<Lock> l(lock);
                counter++;
                std::cout << "unique_lock begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "unique_lock end" << counter << std::endl;
            }
        });

        std::thread t4([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            std::cout << "t4" << std::endl;
            {
                std::unique_lock<Lock> l(lock);
                counter++;
                std::cout << "unique_lock2 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "unique_lock2 end" << counter << std::endl;
            }
        });

        std::thread t5([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "t5" << std::endl;
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "shared_lock t5 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                std::cout << "shared_lock t5 end" << counter << std::endl;
            }
        });


        std::thread t6([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            std::cout << "t6" << std::endl;
            {
                std::unique_lock<Lock> l(lock);
                counter++;
                std::cout << "unique_lock6 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "unique_lock6 end" << counter << std::endl;
            }
        });

        std::thread t7([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
            std::cout << "t7" << std::endl;
            {
                std::shared_lock<Lock> l(lock);
                std::cout << "shared_lock7 begin" << counter << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                std::cout << "shared_lock7 end" << counter << std::endl;
            }
        });

        t0.join();
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
    }

    void test_upgrade(){
        using namespace threading;
        using Lock = RWSpinLockWriterBiased<>;
        Lock lock;

        std::cout << "1";
        std::unique_lock<Lock> ul(lock);
        std::cout << "2";
        std::shared_lock<Lock> sl( upgrade_lock<Lock>(std::move(ul)) );
        std::cout << "3";
    }

    void test_all(){
        //test_1();
        test_shared_lock();
        //test_upgrade();
    }
};

#endif //THREADING_TEST_TESTRWSPINLOCK_H
