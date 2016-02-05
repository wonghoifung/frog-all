#include "caf/config.hpp"
#include <arpa/inet.h> // htonl
#include <vector>
#include <string>
#include <limits>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iostream>

#include "caf/all.hpp"
#include "caf/io/all.hpp"

using std::cout;
using std::cerr;
using std::endl;

using namespace caf;
using namespace caf::io;

behavior server(event_based_actor* self) {
	publish(self, 4242);
	self->become(
		on(atom("ping"), arg_match) >> [](int i) {
			return make_message(atom("pong"), i);
		}
	);
}

int main(int argc, char** argv) {
	auto actor = spawn(server);
	await_all_actors_done();
	shutdown();
}
