#include <iostream>

#include "TestRecursiveSpinLock.h"
//#include "TestRWSpinLock.h"
//#include "TestThreadExecutor.h"

int main() {
    TestRecursiveSpinLock().test_all();
    //TestRWSpinLock().test_all();
    //TestThreadExecutor().test_all();

#ifdef _MSC_VER
	char ch;
	std::cin >> ch;
#endif

    return 0;
}