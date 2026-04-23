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
#include "rt_scope/histogram.hpp"
namespace rt_scope
{
    class Worker
    {
    private:
        int coreId;
        std::atomic<bool> *startSignal;
        std::vector<long long> latencies;
        int interval;
        Histogram stats_;
        

    public:
        Worker(int coreId_, std::atomic<bool>* startSignal_, int interval, size_t num_buckets, uint64_t bucket_size_ns);
        Worker(const Worker&) = delete;
        Worker& operator=(const Worker) = delete;
        int run(int inerations);
        int getCore();
        //long long getMaxLatency();
        const Histogram& get_stats() const;
        ~Worker();

    };
}

#endif