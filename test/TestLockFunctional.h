#ifndef THREADING_TEST_TESTLOCKFUNCTIONAL_H
#define THREADING_TEST_TESTLOCKFUNCTIONAL_H

#include <mutex>

#include <threading/lock_functional.h>

struct TestLockFunctional{
    void test_all(){
        using namespace threading;

        std::mutex m1;
        std::mutex m2;

        bool b;

        auto l = lock_functional(
            [&](){ return &m1; },
            [&](){ return (b ? nullptr : &m2); }
        );
    }
};

#endif //THREADING_TEST_TESTLOCKFUNCTIONAL_H
