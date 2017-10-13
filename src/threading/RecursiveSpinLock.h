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
            if (level==0) {
                const bool locked = Base::try_lock();
                if (locked){
                    level++;
                }
                return locked;
            }

            return true;
        }

        void lock(){
            if (level==0){
                Base::lock();
            }

            level++;
        }


        void unlock(){
            level--;

            if (level==0)
                Base::unlock();
        }
    };

}
