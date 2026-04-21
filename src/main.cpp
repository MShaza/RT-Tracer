#ifndef _GNU_SOURCE
#define _GNU_SOURCE 
#endif
#include <iostream>
#include <chrono>
#include "sched.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <thread>
#include <atomic>
#include "rt_scope/worker.hpp"

int main(){
    int iterations =  1000;
   std::vector<std::unique_ptr<rt_scope::Worker>> worker;
   int maxCores = std::thread::hardware_concurrency();
   std::vector<std::thread> threads_;
   std::atomic<bool> startSignal(false);
   //std::cout<<"No of cores: "<<maxCores;
   for(unsigned int core = 0; core < maxCores; core++){
    worker.push_back(std::make_unique<rt_scope::Worker>(core, &startSignal, iterations));
    threads_.emplace_back(&rt_scope::Worker::run, worker.back().get());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Triggering measurement across all cores..." << std::endl;
    startSignal.store(true, std::memory_order_release);
   }
   for(auto &thread:threads_){
    if(thread.joinable()){
        thread.join();
    }
   }
   std::cout << "\n--- Final Latency Results (ns) ---" << std::endl;
    for (const auto& w : worker) {
        printf("Core %2d: Max Jitter = %lld ns\n", w->getCore(), w->getMaxLatency());
    }
    return 0;
}