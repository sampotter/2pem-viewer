#ifndef __ASIO_STATE_HPP__
#define __ASIO_STATE_HPP__

#include <dsp/frame.hpp>
#include <glclient/options.hpp>

struct asio_state {
    ~asio_state();
    static asio_state from_options(options const & options);
    void receive_frame(frame const & f);
private:
    asio_state(options const & options);
    struct impl;
    impl * m_;
};

#endif // __ASIO_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
