#ifndef __APP_STATE_HPP__
#define __APP_STATE_HPP__

#include <glclient/options.hpp>

struct app_state {
    ~app_state();
    static app_state from_options(options const & options);
    static app_state from_cli_args(int argc, char ** argv);
    void run();
private:
    app_state(options const & options);
    struct impl;
    impl * _m;
};

#endif // __APP_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
