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

behavior client(event_based_actor* self) {
	auto server = remote_actor("localhost", 4242);
	self->send(server, atom("ping"), 0);
	self->become(
		on(atom("pong"), 10) >> [=] {
			self->quit();
		},
		on(atom("pong"), arg_match) >> [=](int i) {
			return make_message(atom("ping"), i + 1);
		}
	);
}

int main(int argc, char** argv) {
	auto actor = spawn(client);
	await_all_actors_done();
	shutdown();
}
