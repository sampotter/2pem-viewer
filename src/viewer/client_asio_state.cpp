#include "client_asio_state.hpp"

void
client_asio_state::connect(client_options const & options)
{
    using namespace boost::asio;
    using namespace boost::asio::ip;

	auto const hostname = options.get_hostname();
	auto const port = options.get_port();
	
    tcp::resolver resolver(io_);
    tcp::resolver::query query(hostname.c_str(), port.c_str());
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::connect(socket_, endpoint_iterator);
}

void
client_asio_state::receive_frame(frame const & f, client_error & error)
{
    boost::system::error_code error_code;
	auto const data = static_cast<void *>(const_cast<GLfloat *>(f.data()));
	auto const num_bytes = sizeof(GLfloat)*f.size();
    auto buffer = boost::asio::buffer(data, num_bytes);
    boost::asio::read(socket_, buffer, error_code);
	switch (error_code.value()) {
	case boost::system::errc::success:
		error = client_error::success;
		break;
	default:
		error = client_error::net_read_failure;
		break;
	}
}

void
client_asio_state::disconnect()
{
    boost::system::error_code error_code;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    socket_.close();
    io_.stop();
}
