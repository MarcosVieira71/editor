#include "ThreadPool.h"

#include <stdexcept>

ThreadPool& ThreadPool::getInstance(std::size_t num)
{
    if (num == 0)
        num = 1;

    static ThreadPool instance(num);
    return instance;
}


ThreadPool::ThreadPool(std::size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}


void ThreadPool::enqueue(Task t)
{
    {
        std::lock_guard<std::mutex> lock(worker_mtx);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace(t);
    }

    cv.notify_one();
}


ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(worker_mtx);
        stop = true;
    }

    cv.notify_all();

    for (std::thread &worker : workers)
        worker.join();

    {
        std::lock_guard<std::mutex> lock(worker_mtx);
        while (!tasks.empty()) tasks.pop();
    }
    {
        std::lock_guard<std::mutex> lock(mainMtx);
        while (!mainTasks.empty()) mainTasks.pop();
    }
        
}


void ThreadPool::processMainThreadTasks() {
    std::queue<Task> local;

    {
        std::lock_guard<std::mutex> lock(mainMtx);
        std::swap(local, mainTasks);
    }

    while (!local.empty()) {
        local.front()();
        local.pop();
    }
}
void ThreadPool::postToMain(Task t)
{
    std::lock_guard<std::mutex> lock(mainMtx);
    mainTasks.push(std::move(t));
}

void ThreadPool::workerLoop()
{
    while (true) {
        Task task;

        {
            std::unique_lock<std::mutex> lock(worker_mtx);
            cv.wait(lock, [this] {
                return stop || !tasks.empty();
            });

            if (stop && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
