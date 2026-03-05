#pragma once

#include <functional>
#include <vector>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(size_t workers = 4);
    ~ThreadPool();

    void Enqueue(std::function<void()> task);
    void Shutdown();

private:
    std::vector<std::thread> workers_;
};
