#pragma once

#include <functional>

#include "ThreadPool.h"

// template<typename T>
class TaskScheduler {
public:
    static void schedule(std::function<void()> task, std::function<void()> after_task) {
        ThreadPool::getInstance().runAsync(std::move(task), std::move(after_task));
    }

    template<typename T>
    static void schedule(std::function<T()> task, std::function<void(T)> after_task) {
        ThreadPool::getInstance().runAsync(std::move(task), std::move(after_task));
    }
};
