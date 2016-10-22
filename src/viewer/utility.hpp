#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <chrono>
#include <future>

template <class T>
bool is_ready(std::future<T> const & future)
{
	return future.wait_for(std::chrono::seconds(0)) ==
		std::future_status::ready;
}

#endif // __UTILITY_HPP__
