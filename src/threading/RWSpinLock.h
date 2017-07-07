#pragma once

#include <atomic>
#include "SpinLock.h"

namespace threading{
    class RWSpinLock{
        std::atomic<int> readers_count{0};
        std::atomic<bool> write_now{false};
    public:
        void lock() {
            while (write_now.exchange(true, std::memory_order_acquire)){
                std::this_thread::yield();
            }

            // wait for readers to exit
            while (readers_count != 0 ){
                std::this_thread::yield();
            }
        }
        void unlock() {
            write_now.store(false, std::memory_order_release);
        }

        void lock_shared() {
            // unique_lock have priority
            while(true) {
                while (write_now) {     // wait for unlock
                    std::this_thread::yield();
                }

                readers_count.fetch_add(1, std::memory_order_acquire);

                if (write_now){
                    // locked while transaction? Fallback. Go another round
                    readers_count.fetch_sub(1, std::memory_order_release);
                } else {
                    // all ok
                    return;
                }
            }
        }
        void unlock_shared() {
            readers_count.fetch_sub(1, std::memory_order_release);
        }
    };

}