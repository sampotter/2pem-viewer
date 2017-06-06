#ifndef __ASIO_STATE_HPP__
#define __ASIO_STATE_HPP__

#include <boost/asio.hpp>

#include "options.hpp"
#include "frame.hpp"

struct asio_state {
    asio_state(options const & options);
    ~asio_state();
    void connect(options const & options);
    void receive_frame(frame const & f);
    void disconnect();
private:
    boost::asio::io_service io_;
    boost::asio::ip::tcp::socket socket_ {io_};
};

#endif // __ASIO_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
