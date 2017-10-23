#ifndef THREADING_TEST_TESTLEVELCOUNTER_H
#define THREADING_TEST_TESTLEVELCOUNTER_H

#include <iostream>
#include <mutex>

#include <threading/RecursiveLevelCounter.h>
#include <threading/Recursive.h>
#include <threading/SpinLock.h>
#include <threading/RWSpinLock.h>

struct TestRecursiveLevelCounter{
    void test_all(){
        using namespace threading;

        using Lock = RecursiveLevelCounter<Recursive<RWSpinLock>>;
        Lock lock;

        {
            std::unique_lock l1(lock);
            std::unique_lock l2(lock);

            std::cout << lock.level() << std::endl;
        }
    }
};

#endif //THREADING_TEST_TESTLEVELCOUNTER_H
