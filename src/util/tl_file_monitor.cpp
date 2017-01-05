#include "../external/dir_monitor/include/dir_monitor/dir_monitor.hpp"

int main() {
	boost::asio::io_service io;
	boost::asio::dir_monitor dm {io};
}
