#include <chrono>
#include <thread>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

int main() {
	lo::Address addr {"localhost", "9000"};

	for (;;) {
		addr.send("test", "i", 1234);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
