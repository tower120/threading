#ifndef THREADING_TEST_TESTRECURSIVESPINLOCK_H
#define THREADING_TEST_TESTRECURSIVESPINLOCK_H

#include <mutex>
#include <threading/Recursive.h>
#include <threading/RWSpinLock.h>

struct TestRecursiveSpinLock{
    void test_lock_unlock(){
        using namespace threading;
        Recursive<RWSpinLock> lock;

        {
            std::unique_lock l(lock);

            std::unique_lock l2(lock);

        }

    }
    void test_all(){
        test_lock_unlock();
    }
};


#endif //THREADING_TEST_TESTRECURSIVESPINLOCK_H
