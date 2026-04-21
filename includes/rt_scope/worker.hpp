#ifndef WORKER_HPP
#define WORKER_HPP
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 
#endif
#include <atomic>
#include <iostream>
#include <chrono>
#include "sched.h"
#include <vector>
#include <algorithm>
namespace rt_scope
{
    class Worker
    {
    private:
        int coreId;
        std::atomic<bool> *startSignal;
        std::vector<long long> latencies;
        int interval;
        

    public:
        Worker(int coreId_, std::atomic<bool>* startSignal_, int interval);
        Worker(const Worker&) = delete;
        Worker& operator=(const Worker) = delete;
        int run();
        int getCore();
        long long getMaxLatency();
        ~Worker();

    };
}

#endif