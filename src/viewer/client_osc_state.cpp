#include "client_osc_state.hpp"

#include <iostream>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

struct client_osc_state::impl {
	impl(std::size_t port);
	lo::ServerThread server_thread_;
};

client_osc_state::impl::impl(std::size_t port): server_thread_ {port}
{
	server_thread_.add_method(
		"test",
		"i",
		[] (lo_arg ** argv, int i) {
			std::cout << argv[0]->i << ", " << i << std::endl;
		});
	server_thread_.start();
}

client_osc_state::client_osc_state(std::size_t port):
	impl_ {std::make_unique<impl>(port)}
{}

client_osc_state::~client_osc_state()
{}
