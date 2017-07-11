#pragma once

#include <atomic>

namespace threading {
namespace experimental {

    // unfinished
    // from https://blogs.oracle.com/dave/waiting-policies-for-locks-:-spin-then-park
    // and  https://people.mpi-sws.org/~bbb/papers/pdf/rtsj11.pdf
    class RWSpinLockBrandenburg{
        std::atomic<int> rin;
        std::atomic<int> rout;
        SpinLock<SpinLockMode::Yield> writeLock;
        int phase;
    public:

        void lock() {
            writeLock.lock();

            int p = (++phase) & 1;
            int tx = rin + p|2;
            while(((tx ^ rout) & ~3) != 0){
                std::this_thread::yield();
            }
        }

        void unlock() {

            writeLock.unlock();
        }

        void lock_shared() {
            int w = rin.fetch_add(0x100) & 3;
            if (w!=0){
                while(w == rin & 3 ){
                    std::this_thread::yield();
                }
            }
        }
        void unlock_shared() {
            rout.fetch_add(0x100);
        }
    };


}
}