#include "rt_scope/histogram.hpp"

namespace rt_scope
{
    Histogram::Histogram(size_t noOfBuckets_, uint64_t bucketSize_) : buckets_(noOfBuckets_, 0),
                                                                      sizeOfBuckets(bucketSize_),
                                                                      totalSamples(0),
                                                                      min_ns(UINT64_MAX),
                                                                      max_ns(0) {}

    void Histogram::record(uint64_t latency_ns)
    {
        totalSamples++;
        min_ns = std::min(min_ns, latency_ns);
        max_ns = std::max(max_ns, latency_ns);
        size_t index = static_cast<size_t>(latency_ns / sizeOfBuckets);
        if (index < buckets_.size())
        {
            buckets_[index]++;
        }
        else
        {
            _overflow++;
        }
    }
    uint64_t Histogram::get_percentile(double percentile) const
    {

        if (totalSamples == 0)
            return 0;

        double target = totalSamples * (percentile / 100.0);

        uint64_t side_sum = 0;

        for (size_t i = 0; i < buckets_.size(); ++i)
        {
            side_sum += buckets_[i];

            if (static_cast<double>(side_sum) >= target)
            {

                return static_cast<uint64_t>(i) * sizeOfBuckets;
            }
        }

        return max_ns;
    }
    double Histogram::get_mean() const {
        if (totalSamples == 0) return 0.0;
        uint64_t sum_ns = 0;
        for (size_t i = 0; i < buckets_.size(); ++i) {
            uint64_t midpoint = (i * sizeOfBuckets) + (sizeOfBuckets / 2);
            sum_ns += buckets_[i] * midpoint;
        }
        sum_ns += _overflow * max_ns;
        return static_cast<double>(sum_ns) / totalSamples;
    }

    uint64_t Histogram::get_min() const { return min_ns== std::numeric_limits<uint64_t>::max() ? 0 : min_ns; }
    uint64_t Histogram::get_max() const { return max_ns; }
    uint64_t Histogram::get_total_samples() const { return totalSamples; }
}
