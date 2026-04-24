#include "rt_scope/worker.hpp"

namespace rt_scope
{
    Worker::Worker(int coreId_, std::atomic<bool> *startSignal_, int interval, size_t num_buckets, uint64_t bucket_size_ns) : coreId(coreId_), startSignal(startSignal_), stats_(num_buckets, bucket_size_ns)
    {
        latencies.reserve(interval);
    }
    Worker::~Worker()
    {
    }
    int Worker::run(int iterations)
    {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(coreId, &mask);
        if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
        {
            perror("sched_setaffinity");
            return -1;
        }
        struct sched_param param;
        param.sched_priority = 95;
        if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0)
        {
            perror("pthread_setschedparam failed (try running with sudo)");
        }
        while (!startSignal->load(std::memory_order_acquire))
        {
#if defined(__i386__) || defined(__x86_64__)
            asm volatile("pause" ::: "memory"); // x86 hint to reduce power during spin
#endif
        }
        auto const interval = std::chrono::microseconds(100);
        for (int i = 0; i < iterations; i++)
        {
            auto startTime = std::chrono::steady_clock::now();
            auto targetime = startTime + interval;
            while (std::chrono::steady_clock::now() < targetime)
            {
                __builtin_ia32_pause();
            }
            auto actualEnd = std::chrono::steady_clock::now();
            auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(actualEnd - targetime);
            stats_.record(static_cast<uint64_t>(latency.count()));
        } // latencies.push_back(latency.count());
        return 0;
    }

    /**long long Worker::getMaxLatency(){
          auto max_latency = *std::max_element(latencies.begin(), latencies.end());
          return max_latency;
     }**/
    int Worker::getCore()
    {
        return coreId;
    }
    const Histogram &Worker::get_stats() const
    {
        return stats_;
    }

}
