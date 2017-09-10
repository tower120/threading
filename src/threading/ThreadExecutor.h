#pragma once

#include <thread>
#include <functional>
#include <mutex>

#include "SpinLock.h"

namespace threading{

    template<class Work = std::function<void()> >
    class ThreadExecutor{
        inline static thread_local ThreadExecutor<Work>* active_thread_executor = nullptr;
        static const constexpr bool execute_from_same_thread_optimisation = true;

        std::thread thread;

        using List = std::vector<Work>;
        List list;

        using Lock = SpinLock<>;
        Lock lock;

        bool thread_alive{true};
    public:
        ThreadExecutor(const ThreadExecutor&) = delete;
        ThreadExecutor(ThreadExecutor&&) = delete;

        ThreadExecutor()
            :thread([&](){
                active_thread_executor = this;

                List closures;
                while(true) {
                    {
                        std::unique_lock<Lock> l(lock);

                        if (!thread_alive) break;

                        if (list.empty()){
                            l.unlock();
                            std::this_thread::yield();
                            continue;
                        }

                        closures = std::move(list);
                        list.clear();
                    }

                    for (Work &work : closures) {
                        work();
                    }
                }
            })
        {}


        template<class Closure>
        void execute(Closure&& closure){
            if (execute_from_same_thread_optimisation
                && active_thread_executor == this)
            {
                closure();
                return;
            }

            std::unique_lock<Lock> l(lock);
            list.emplace_back(std::forward<Closure>(closure));
        }

        template<class Closure>
        void operator<<(Closure&& closure){
            execute(std::forward<Closure>(closure));
        }

        ~ThreadExecutor(){
            {
                std::unique_lock<Lock> l(lock);
                thread_alive = false;
            }
            thread.join();
        }
    };
}