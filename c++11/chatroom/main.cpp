#include "caf/config.hpp"

#ifdef WIN32
# define _WIN32_WINNT 0x0600
# include <Winsock2.h>
#else
# include <arpa/inet.h> // htonl
#endif

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

using ping_atom = atom_constant<atom("ping")>;
using pong_atom = atom_constant<atom("pong")>;
using kickoff_atom = atom_constant<atom("kickoff")>;

void print_on_exit(const actor& hdl, const std::string& name) {
	hdl->attach_functor([=](abstract_actor* ptr, uint32_t reason) {
		aout(ptr) << "E--------> " << name << " exited with reason " << reason << endl;
	});
}

behavior ping(event_based_actor* self, size_t num_pings) {
	auto count = std::make_shared<size_t>(0);
	return{
		[=](kickoff_atom, const actor& remote_pong) {
			aout(self) << ">>>>>>>>>>>>>>> ping" << endl;
			self->send(remote_pong, ping_atom::value, int32_t(1));
			self->become(
			[=](pong_atom, int32_t value) -> message {
				if (++*count >= num_pings) self->quit();
				return make_message(ping_atom::value, value + 1);
			}
			);
		}
	};
}

behavior pong() {
	return{
		[](ping_atom, int32_t value) {
			cout << ">>>>>>>>>>>>>>>>>>> pong" << endl;
			return make_message(pong_atom::value, value);
		}
	};
}

template <class T>
void write_int(broker* self, connection_handle hdl, T value) {
	using unsigned_type = typename std::make_unsigned<T>::type;
	auto cpy = static_cast<T>(htonl(static_cast<unsigned_type>(value)));
	self->write(hdl, sizeof(T), &cpy);
	self->flush(hdl);
}

void write_int(broker* self, connection_handle hdl, uint64_t value) {
	write_int(self, hdl, static_cast<uint32_t>(value));
	write_int(self, hdl, static_cast<uint32_t>(value >> sizeof(uint32_t)));
}

template <class T>
void read_int(const void* data, T& storage) {
	using unsigned_type = typename std::make_unsigned<T>::type;
	memcpy(&storage, data, sizeof(T));
	storage = static_cast<T>(ntohl(static_cast<unsigned_type>(storage)));
}

void read_int(const void* data, uint64_t& storage) {
	uint32_t first;
	uint32_t second;
	read_int(data, first);
	read_int(reinterpret_cast<const char*>(data)+sizeof(uint32_t), second);
	storage = first | (static_cast<uint64_t>(second) << sizeof(uint32_t));
}

behavior broker_impl(broker* self, connection_handle hdl, const actor& buddy) {
	assert(self->num_connections() == 1);
	self->monitor(buddy);
	self->configure_read(hdl, receive_policy::exactly(sizeof(uint64_t) + sizeof(int32_t)));
	return{
		[=](const connection_closed_msg& msg) {
			if (msg.handle == hdl) {
				aout(self) << "connection closed" << endl;
				self->send_exit(buddy, exit_reason::remote_link_unreachable);
				self->quit(exit_reason::remote_link_unreachable);
			}
		},
		[=](atom_value av, int32_t i) {
			assert(av == ping_atom::value || av == pong_atom::value);
			aout(self) << "send {" << to_string(av) << ", " << i << "}" << endl;
			write_int(self, hdl, static_cast<uint64_t>(av));
			write_int(self, hdl, i);
		},
		[=](const down_msg& dm) {
			if (dm.source == buddy) {
				aout(self) << "our buddy is down" << endl;
				self->quit(dm.reason);
			}
		},
		[=](const new_data_msg& msg) {
			uint64_t atm_val;
			read_int(msg.buf.data(), atm_val);
			auto atm = static_cast<atom_value>(atm_val);
			int32_t ival;
			read_int(msg.buf.data() + sizeof(uint64_t), ival);
			aout(self) << "received {" << to_string(atm) << ", " << ival << "}" << endl;
			self->send(buddy, atm, ival);
		},
		others >> [=] {
			aout(self) << "unexpected: " << to_string(self->current_message()) << endl;
		}
	};
}

behavior server(broker* self, const actor& buddy) {
	aout(self) << "server is running" << endl;
	return{
		[=](const new_connection_msg& msg) {
			aout(self) << "server accepted new connection" << endl;
			auto impl = self->fork(broker_impl, msg.handle, buddy);
			print_on_exit(impl, "broker_impl");
			//aout(self) << "quit server (only accept 1 connection)" << endl;
			//self->quit();
		},
		others >> [=] {
			aout(self) << "unexpected: " << to_string(self->current_message()) << endl;
		}
	};
}

//optional<uint16_t> as_u16(const std::string& str) { return static_cast<uint16_t>(stoul(str)); }

int main(int argc, char** argv) {
	using std::string;
	uint16_t port = 0;
	string host = "localhost";
	auto res = message_builder(argv + 1, argv + argc).extract_opts({
		{ "server,s", "run in server mode" },
		{ "client,c", "run in client mode" },
		{ "port,p", "set port", port },
		{ "host,H", "set host (client mode only" }
	});
	if (!res.error.empty()) {
		cerr << res.error << endl;
		return 1;
	}
	if (res.opts.count("help") > 0) {
		cout << res.helptext << endl;
		return 0;
	}
	if (!res.remainder.empty()) {
		cerr << "*** too many arguments" << endl << res.helptext << endl;
		return 1;
	}
	if (res.opts.count("port") == 0) {
		cerr << "*** no port given" << endl << res.helptext << endl;
		return 1;
	}
	if (res.opts.count("server") > 0) {
		cout << "run in server mode" << endl;
		auto pong_actor = spawn(pong);
		auto server_actor = spawn_io_server(server, port, pong_actor);
		print_on_exit(server_actor, "server");
		print_on_exit(pong_actor, "pong");
	}
	else if (res.opts.count("client") > 0) {
		auto ping_actor = spawn(ping, size_t{ 1 });
		auto io_actor = spawn_io_client(broker_impl, host, port, ping_actor);
		print_on_exit(io_actor, "protobuf_io");
		print_on_exit(ping_actor, "ping");
		send_as(io_actor, ping_actor, kickoff_atom::value, io_actor);
	}
	else {
		cerr << "*** neither client nor server mode set" << endl << res.helptext << endl;
		return 1;
	}
	await_all_actors_done();
	shutdown();
}
