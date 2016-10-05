#ifndef __SCOPE_EXIT_HPP__
#define __SCOPE_EXIT_HPP__

struct scope_exit {
	scope_exit(std::function<void(void)> func): func_ {func} {};
	~scope_exit() { func_(); }
private:
	std::function<void(void)> func_;
};

#endif // __SCOPE_EXIT_HPP__
