#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {

public:
    static ThreadPool& getInstance(std::size_t num = std::thread::hardware_concurrency());

    using Task = std::function<void()>;


    template<typename F, typename C>
    void runAsync(F&& background, C&& onComplete)
    {
        using Result = std::invoke_result_t<F>;

        enqueue(
            [this,
            bg = std::forward<F>(background),
            cb = std::forward<C>(onComplete)]() mutable
            {
                try {
                    if constexpr (std::is_void_v<Result>) {
                        bg();

                        postToMain([cb = std::move(cb)]() mutable {
                            cb();
                        });
                    }
                    else {
                        Result result = bg();

                        postToMain(
                            [cb = std::move(cb),
                            result = std::move(result)]() mutable {
                                cb(std::move(result));
                            }
                        );
                    }
                }
                catch (...) {
                    auto err = std::current_exception();
                    postToMain([cb = std::move(cb), err]() mutable {
                        if constexpr (std::is_void_v<Result>) {
                            cb(); 
                        } else {
                            cb(Result{});
                        }
                    });
                }
            }
        );
    }
        
    void processMainThreadTasks();

    ~ThreadPool();

private:


    static ThreadPool* instance;
    explicit ThreadPool(std::size_t num);
    
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    void workerLoop();
    void enqueue(Task t);
    void postToMain(Task task);

    std::vector<std::thread> workers;
    std::queue<Task> tasks;
    std::mutex worker_mtx;

    std::queue<Task> mainTasks;
    std::mutex mainMtx;

    std::condition_variable cv;
    bool stop = false;


};
