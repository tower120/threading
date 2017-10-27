#ifndef THREADING_TEST_TESTLOCKFUNCTIONAL_H
#define THREADING_TEST_TESTLOCKFUNCTIONAL_H

#include <mutex>

#include <threading/lock_functional.h>

struct TestLockFunctional{
    void test_simple(){
        using namespace threading;

        std::mutex m1;
        std::mutex m2;

        bool b;

        auto l = lock_functional(
            [&](){ return &m1; },
            [&](){ return (b ? nullptr : &m2); }
        );
    }

    void test_lock_all(){
        using namespace threading;

        std::mutex m1;
        std::mutex m2;

        bool b{true};

        auto l = lock_all_functional(
            [&](){ return &m1; },
            [&](){ return (b ? nullptr : &m2); }
        );

        if (!l){
            std::cout << "Lock fail" << std::endl;
        }
    }
    void test_all(){
        //test_simple();
        test_lock_all();
    }
};

#endif //THREADING_TEST_TESTLOCKFUNCTIONAL_H
