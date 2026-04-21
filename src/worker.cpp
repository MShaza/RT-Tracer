#include "rt_scope/worker.hpp"

namespace rt_scope
{
    Worker::Worker(int coreId_, std::atomic<bool>*startSignal_, int interval):coreId(coreId_) , startSignal(startSignal_)
    {
        latencies.reserve(interval);
    }
     Worker::~Worker()
    {
    }
    int Worker::run(){
         cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(coreId, &mask);
        if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1){
        perror("sched_setaffinity");
        return -1;
    }
    while(!startSignal->load(std::memory_order_acquire)){
        #if defined(__i386__) || defined(__x86_64__)
            asm volatile("pause" ::: "memory"); // x86 hint to reduce power during spin
        #endif
    }
    auto const interval = std::chrono::microseconds(100);
    auto startTime = std::chrono::steady_clock::now();
    auto targetime = startTime + interval;
            while(std::chrono::steady_clock::now() < targetime){
                __builtin_ia32_pause();
            }
        auto actualEnd = std::chrono::steady_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(actualEnd - targetime);
        latencies.push_back(latency.count());
     return 0;
    }

    long long Worker::getMaxLatency(){
         auto max_latency = *std::max_element(latencies.begin(), latencies.end());
         return max_latency;
    }
    int Worker::getCore(){
        return coreId;
    }

}

