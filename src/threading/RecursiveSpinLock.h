#pragma once

#include "SpinLock.h"

namespace threading{

    template<SpinLockMode mode = SpinLockMode::Adaptive>
    class RecursiveSpinLock : public SpinLock<mode>{
        using Base = SpinLock<mode>;
        inline static thread_local std::size_t level{0};
    public:
        using Base::Base;

        bool try_lock(){
            level++;

            if (level==1)
                Base::try_lock();
        }

        void lock(){
            level++;

            if (level==1)
                Base::lock();
        }


        void unlock(){
            level--;

            if (level==0)
                Base::unlock();
        }
    };

}
