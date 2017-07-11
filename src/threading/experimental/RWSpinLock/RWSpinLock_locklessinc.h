#pragma once

#include <atomic>

namespace threading{
namespace experimental {

    // from #https://locklessinc.com/articles/locks/
    // Seems the same as WrierWise, but slower on shared_lock, due to opportunistic approach
    class RWSpinLock_locklessinc{
        std::atomic<int>  readers_count{0};
        std::atomic<bool> write_now{false};
    public:
        void lock() {
            while (write_now.exchange(true)){
                std::this_thread::yield();
            }

            // wait for readers to exit
            while (readers_count.load() != 0 ){
                std::this_thread::yield();
            }
        }
        void unlock() {
            write_now.store(false);
        }

        void lock_shared() {
            while(true) {
                readers_count.fetch_add(1);

                if (!write_now) return;

                readers_count.fetch_sub(1);
                std::cout << "!";

                while (write_now) {     // wait for unlock
                    std::this_thread::yield();
                }
            }
        }
        void unlock_shared() {
            readers_count.fetch_sub(1);
        }
    };

}
}