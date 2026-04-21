#ifndef _GNU_SOURCE
#define _GNU_SOURCE 
#endif
#include <iostream>
#include <chrono>
#include "sched.h"
#include <vector>
#include <algorithm>

int pinToCore(int coreNumber){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(coreNumber, &mask);
    if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1){
        perror("sched_setaffinity");
        return -1;
    }
     return 0;
}
auto getLatency(std::chrono::nanoseconds interval){
    auto startTime = std::chrono::steady_clock::now();
    auto targetime = startTime + interval;
            while(std::chrono::steady_clock::now() < targetime){
                __builtin_ia32_pause();
            }
        auto actualEnd = std::chrono::steady_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::nanoseconds>(actualEnd - targetime);
        return latency.count(); 
}

int main(){
    int iterations =  1000;
    auto interval = std::chrono::milliseconds(1);
    std::vector<long long> results;
    results.reserve(1000);
    if(pinToCore(0) != 0){
        std::cout<<"Core not pinned"<<std::endl;
        return 1;
    }
    for(int i = 0; i < iterations; i++ ){
        results.push_back(getLatency(interval));
    }
    auto max_latency = *std::max_element(results.begin(), results.end());
    std::cout << "Max Latency: " << max_latency << " ns" << std::endl;
    return 0;
}