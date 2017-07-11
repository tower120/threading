#pragma once

#include <atomic>

namespace threading{

    // use by default. Updates should be prioritised. Reader may never have a chance, if there is always writer queue.
    // Safe to use if lock -> shared_lock (updates) always.
    // Use only one cmpxchg for lock/unlock
    // and one "lock; xaddl" for shared_lock/shared_unlock
    class RWSpinLockWriterBiased{
        std::atomic<int>  readers_count{0};
        std::atomic<bool> write_now{false};
    public:
        void lock() {
            while (write_now.exchange(true, std::memory_order_acquire)){
                std::this_thread::yield();
            }

            // wait for readers to exit
            while (readers_count.load(std::memory_order_acquire) != 0 ){
                std::this_thread::yield();
            }
        }
        void unlock() {
            write_now.store(false, std::memory_order_release);
        }

        void lock_shared() {
            while(true) {
                while (write_now.load(std::memory_order_acquire)) {     // wait for unlock
                    std::this_thread::yield();
                }

                // Safe to use acquire instead of acq_rel because of RMW operation https://stackoverflow.com/questions/21536846/is-memory-order-acquire-really-sufficient-for-locking-a-spinlock#comment32531346_21537024
                // Per 29.3/12 "Atomic read-modify-write operations shall always read the last value
                // (in the modification order) written before the write associated with the read-modify-write operation."
                // The memory ordering you select for such an R-M-W op only affects how it is ordered with reads/writes to other memory locations and non R-M-W ops to the same atomic.
                readers_count.fetch_add(1, std::memory_order_acquire);

                // Very rare case
                if (!write_now.load(std::memory_order_acquire)){
                    // all ok
                    return;
                } else {
                    // locked while "transaction"? Fallback. Go another round
                    unlock_shared();
                }
            }
        }

        void unlock_shared() {
            readers_count.fetch_sub(1, std::memory_order_release);
        }
    };


    // Same as WriterBiased, but reader biased. Writer may never have a chance, if there is always reader queue.
    // lock -> shared_lock update will not help here.
    class RWSpinLockReaderBiased {
        std::atomic<int> readers_count{0};
        std::atomic<bool> write_now{false};
    public:
        void lock() {
            while(true) {
                // wait for readers to exit first
                while (readers_count.load(std::memory_order_acquire) != 0) {
                    std::this_thread::yield();
                }

                while (write_now.exchange(true, std::memory_order_acquire)) {
                    std::this_thread::yield();
                }

                // Rare case
                if (readers_count.load(std::memory_order_acquire) == 0) {
                    return;     // all ok
                } else {
                    unlock();   // Fallback, give a way to readers
                }
            }
        }
        void unlock() {
            write_now.store(false, std::memory_order_release);
        }

        void lock_shared() {
            readers_count.fetch_add(1, std::memory_order_acquire);

            // wait for unlock
            while (write_now.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
        }
        void unlock_shared() {
            readers_count.fetch_sub(1, std::memory_order_release);
        }
    };


    using RWSpinLock = RWSpinLockWriterBiased;
}