#ifndef THREADING_TEST_TESTTHREADEXECUTOR_H
#define THREADING_TEST_TESTTHREADEXECUTOR_H

#include <threading/ThreadExecutor.h>
#include <iostream>

struct TestThreadExecutor{

    void test_2_executors(){
        using namespace threading;

        struct Data{
            ThreadExecutor<>* on_thread;
        };


        ThreadExecutor main_thread, thread1, thread2;

        main_thread << [&](){
            thread1 << [&]() {
                std::cout << "do work on thread 1" << std::endl;
            };
            thread2 << [&]() {
                std::cout << "do work on thread 2" << std::endl;
            };
        };
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void test_1(){
        using namespace threading;
        ThreadExecutor executor;

        executor.execute([](){
            std::cout << "gav" << std::endl;
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));

        executor << [](){
            std::cout << "muu!!!" << std::endl;
        };

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    void test_all(){
        //test_1();
        test_2_executors();
    }
};

#endif //THREADING_TEST_TESTTHREADEXECUTOR_H
