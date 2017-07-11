#pragma once

#include <thread>

namespace threading {
    enum class SpinLockMode { Nonstop, Yield, Sleep, Adpative };

namespace details{
namespace SpinLockSpinner{

    template<SpinLockMode mode = SpinLockMode::Adpative, class Closure>
    static void spinWhile(Closure&& closure){
        if (mode == SpinLockMode::Adpative) {
            // fast return
            if (!closure()) {
                return;
            }

            while (true) {
                // 1. first as spinner
                for (int i = 0; i < 40; i++) {
                    if (!closure()) {
                        return;
                    }
                }

                // 2. then add some yield
                for (int i = 0; i < 100; i++) {
                    if (!closure()) {
                        return;
                    }
                    std::this_thread::yield();
                }

                // 3. then go to sleep
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            }

            return;
        }


        while (true) {
            if (!closure()) {
                return;
            }
            if (mode == SpinLockMode::Sleep) {
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            }
            else if (mode == SpinLockMode::Yield) {
                std::this_thread::yield();
            }
        }
    }

}
}
}