#ifndef __CLIENT_ASIO_STATE_HPP__
#define __CLIENT_ASIO_STATE_HPP__

#include <boost/asio.hpp>

#include "client_options.hpp"
#include "frame.hpp"

struct client_asio_state {
	client_asio_state(client_options const & options);
	~client_asio_state();
	void connect(client_options const & options);
	void receive_frame(frame const & f);
	void disconnect();
private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_ {io_};
};

#endif // __CLIENT_ASIO_STATE_HPP__
