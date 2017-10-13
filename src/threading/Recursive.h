#pragma once

/// Use as follow:
///
///     Recursive<SpinLock>
///     Recursive<RWSpinLock>

namespace threading{

    template<class spin_lock_t>
    class Recursive : public spin_lock_t{
        using Base = spin_lock_t;
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
