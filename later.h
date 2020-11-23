//
// Created by Hugo Ziviani on 11/23/20.
//based on https://www.tutorialspoint.com/how-to-create-timer-using-cplusplus11

#ifndef KNAPSACK_BOUND_LATER_H
#define KNAPSACK_BOUND_LATER_H
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

class later {
public:
    template <class callable, class... arguments>
    later(int after, bool async, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        if (async) {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }
};
#endif //KNAPSACK_BOUND_LATER_H
