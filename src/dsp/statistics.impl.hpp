#ifndef __STATISTICS_IMPL_HPP_HPP__
#define __STATISTICS_IMPL_HPP_HPP__

template <class T>
online_mean<T>::online_mean(int num_samples): num_samples_ {num_samples} {}

template <class T>
void
online_mean<T>::add_sample(T const & t)
{
    last_samples_.push(t);
    if (samples_counted_ == 0) {
        current_mean_ = t/num_samples_;
    } else if (samples_counted_ < num_samples_) {
        current_mean_ += t/num_samples_;
    } else {
        current_mean_ += (t - last_samples_.front())/num_samples_;
        last_samples_.pop();
    }
    ++samples_counted_;
}

template <class T>
bool
online_mean<T>::has_mean() const
{
    return samples_counted_ >= num_samples_;
}

template <class T>
T
online_mean<T>::get_mean() const
{
    return current_mean_;
}

#endif // __STATISTICS_IMPL_HPP_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
