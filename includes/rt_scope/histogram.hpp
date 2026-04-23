#ifndef _HISTOGRAM_HPP
#define _HISTOGRAM_HPP
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <limits>
namespace rt_scope
{
    class Histogram
    {
    private:
        std::vector<uint64_t> buckets_;
        uint64_t sizeOfBuckets;
        uint64_t totalSamples;
        uint64_t _overflow = 0;
        uint64_t min_ns;
        uint64_t max_ns;

    public:
        Histogram(size_t noOfBuckets_, uint64_t bucketSize_);
        void record(uint64_t latency_ns);
        uint64_t get_percentile(double percentile) const;
        double get_mean() const;
        uint64_t get_max() const;
        uint64_t get_min() const;
        uint64_t get_total_samples() const;
    };
}

#endif