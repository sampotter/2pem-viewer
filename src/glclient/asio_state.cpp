#include "asio_state.hpp"

#include <boost/asio.hpp>
#include <string>

struct asio_state::impl {
    impl(options const & options);
    ~impl();
    void connect();
    void receive_frame(frame const & f);
    void disconnect();

    std::string hostname_;
    std::string port_;
    boost::asio::io_service io_;
    boost::asio::ip::tcp::socket socket_ {io_};
};

asio_state::impl::impl(options const & options):
    hostname_ {options.get_hostname()},
    port_ {options.get_port()}
{
    connect();
}

asio_state::impl::~impl()
{
    disconnect();
}

void
asio_state::impl::connect()
{
    using namespace boost::asio;
    using namespace boost::asio::ip;

    tcp::resolver resolver(io_);
    tcp::resolver::query query(hostname_.c_str(), port_.c_str());
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::connect(socket_, endpoint_iterator);
}

void
asio_state::impl::receive_frame(frame const & f)
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
asio_state::impl::disconnect()
{
    boost::system::error_code error_code;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    socket_.close();
    io_.stop();
}

asio_state
asio_state::from_options(options const & options)
{
    return {options};
}

asio_state::asio_state(options const & options): m_ {new impl(options)} {}

asio_state::~asio_state()
{
    delete m_;
}

void
asio_state::receive_frame(frame const & f)
{
    m_->receive_frame(f);
}

// Local Variables:
// indent-tabs-mode: nil
// End:
