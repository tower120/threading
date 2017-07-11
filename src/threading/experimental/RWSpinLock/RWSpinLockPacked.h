#pragma once

#include <atomic>

namespace threading{
namespace experimental{

    // slower on lock, due to compare_exchange op.
    class RWSpinLockPacked{
        using Bits = int32_t;
        static const constexpr Bits WRITER = 0b1;
        static const constexpr Bits READER = 0b10;

        std::atomic<Bits> bits{0};
    public:
        void lock() {
            // wait for 0 writers, 0 readers, 0.
            while(true){
                Bits expect = 0;
                if (bits.compare_exchange_weak(expect, WRITER)) return;
                std::this_thread::yield();
            }
        }
        void unlock() {
            bits.fetch_and(~WRITER);
        }

        void lock_shared() {
            while(true) {
                while (bits & WRITER) {
                    std::this_thread::yield();
                }

                Bits value = bits.fetch_add(READER);

                if (value & WRITER) {
                    bits.fetch_add(-READER);
                } else {
                    return;
                }
            }
        }
        void unlock_shared() {
            bits.fetch_add(-READER);
        }

        /*void unlock_and_lock_shared() {
            bits_.fetch_add(READER, std::memory_order_acquire);
            unlock();
        }*/
    };

}
}
