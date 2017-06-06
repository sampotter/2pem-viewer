#include "asio_state.hpp"

asio_state::asio_state(options const & options)
{
    connect(options);
}

asio_state::~asio_state()
{
    disconnect();
}

void
asio_state::connect(options const & options)
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
asio_state::receive_frame(frame const & f)
{
    boost::system::error_code error_code;
    auto const data = static_cast<void *>(const_cast<float *>(f.data()));
    auto const num_bytes = sizeof(float)*f.size();
    auto buffer = boost::asio::buffer(data, num_bytes);
    boost::asio::read(socket_, buffer, error_code);
    if (error_code.value() != boost::system::errc::success) {
        throw error_code.value();
    }
}

void
asio_state::disconnect()
{
    boost::system::error_code error_code;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    socket_.close();
    io_.stop();
}

// Local Variables:
// indent-tabs-mode: nil
// End:
