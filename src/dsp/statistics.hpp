#ifndef __STATISTICS_HPP_HPP__
#define __STATISTICS_HPP_HPP__

#include <queue>

template <class T>
struct online_mean {
    online_mean(int num_samples);
    void add_sample(T const & t);
    bool has_mean() const;
    T get_mean() const;
private:
    T current_mean_ {};
    std::queue<T> last_samples_;
    int num_samples_;
    int samples_counted_ {0};
};

#include "statistics.impl.hpp"

#endif // __STATISTICS_HPP_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
