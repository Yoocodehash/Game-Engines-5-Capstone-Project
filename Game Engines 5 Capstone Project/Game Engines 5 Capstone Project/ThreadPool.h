#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <future>

class ThreadPool 
{
private:
    std::mutex mutex;
    std::condition_variable conditionVariable;
    std::deque<std::packaged_task<void()>> work;
    std::vector<std::future<void>> finished;

public:
    template<class F, class R = std::result_of_t<F& ()>>
    std::future<R> queue(F&& f) 
    {
        std::packaged_task<R()> package(std::forward<F>(f));

        auto r = package.get_future();
        {
            std::unique_lock<std::mutex> lock(mutex);
            work.emplace_back(std::move(package));
        }
        conditionVariable.notify_one();

        return r;
    }

    void Start(std::size_t N = 1)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            finished.push_back(std::async(std::launch::async, [this] { ThreadTask(); }));
        }
    }

    void Abort() 
    {
        CancelPending();
        Finish();
    }

    void CancelPending()
    {
        std::unique_lock<std::mutex> lock(mutex);
        work.clear();
    }
    
    void Finish() 
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            for (auto&& unused : finished)
            {
                work.push_back({});
            }
        }
      conditionVariable.notify_all();
      finished.clear();
    }
    
    ~ThreadPool()
    {
        Finish();
    }

private:
    void ThreadTask() {
        while (true)
        {
            std::packaged_task<void()> future;
            {
                std::unique_lock<std::mutex> lock(mutex);
                if (work.empty()) {
                    conditionVariable.wait(lock, [&] {return !work.empty(); });
                }

                future = std::move(work.front());
                work.pop_front();
            }

            if (!future.valid())
            {
                return;
            }

            future();
        }
    }
};