#include "client_app_state.hpp"

int main(int argc, char ** argv) {
	client_app_state::from_cli_args(argc, argv).run();
}

// Local Variables:
// indent-tabs-mode: nil
// End:
