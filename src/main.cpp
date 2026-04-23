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
#include <iomanip>
#include "rt_scope/worker.hpp"

int main(){
    int iterations =  1000;
   std::vector<std::unique_ptr<rt_scope::Worker>> worker;
   int maxCores = std::thread::hardware_concurrency();
   std::vector<std::thread> threads_;
   std::atomic<bool> startSignal(false);
   //std::cout<<"No of cores: "<<maxCores;
   for(int core = 0; core < maxCores; core++){
    worker.push_back(std::make_unique<rt_scope::Worker>(core, &startSignal, iterations, 1000,100));
    threads_.emplace_back(&rt_scope::Worker::run, worker.back().get(), iterations);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Triggering measurement across all cores..." << std::endl;
    startSignal.store(true, std::memory_order_release);
   }
   for(auto &thread:threads_){
    if(thread.joinable()){
        thread.join();
    }
   }
  std::cout << "\n" << std::left << std::setw(8) << "Core" 
              << std::setw(12) << "Min (ns)" 
              << std::setw(12) << "Avg (ns)" 
              << std::setw(12) << "P99 (ns)" 
              << std::setw(12) << "Max (ns)" << "\n";
    std::cout << std::string(56, '-') << "\n";

    for (const auto& w : worker) {
        const auto& s = w->get_stats();
        std::cout << std::left << std::setw(8) << w->getCore() +1
                  << std::setw(12) << s.get_min()
                  << std::setw(12) << std::fixed << std::setprecision(1) << s.get_mean()
                  << std::setw(12) << s.get_percentile(99.0)
                  << std::setw(12) << s.get_max() << "\n";
    }
    return 0;
}