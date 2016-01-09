#include <thread>
#include <chrono>
#include <iostream>
#include <future>

// Per C++11 30.2.6
template <class T> typename std::decay<T>::type decay_copy(T&& v)
{ return std::forward<T>(v); }


class delayed_thread {
	using promise_t = std::promise<void>;
	using thread_t = std::thread;

	enum execution_state {
		WAITING, TRIGGERED, DISMISSED
	};

	promise_t m_promise;
	thread_t m_thread;

	execution_state m_state = WAITING;

public:

	// Construction.

	delayed_thread() = delete;
	delayed_thread(delayed_thread const &) = delete;

	delayed_thread(delayed_thread &&other)
		: m_promise(std::move(other.m_promise))
		, m_thread (std::move(other.m_thread ))
		, m_state (std::move(other.m_state ))
	{
		other.m_state = DISMISSED;
	}

	template<typename op_t>
	delayed_thread(op_t &&op)
	: m_thread([op = decay_copy(std::forward<op_t>(op)), &m_promise = m_promise]() {
		m_promise.get_future().wait();
		op();
	}) {}

	// Destruction.

	~delayed_thread() {
		if(m_state == DISMISSED) {
			return;
		}

		trigger();

		if(m_thread.joinable()) {
			m_thread.join();
		}
	}

	// Assignment.

	delayed_thread &operator = (delayed_thread &&rhs) {
		m_promise = std::move(rhs.m_promise);
		m_thread = std::move(rhs.m_thread );
		m_state = std::move(rhs.m_state );

		rhs.m_state = DISMISSED;
		return *this;
	}

	delayed_thread &operator = (delayed_thread const &rhs) = delete;

	// Execution.

	void trigger() {
		if(m_state == TRIGGERED || m_state == DISMISSED) {
			return;
		}

		m_state = TRIGGERED;
		m_promise.set_value();
	}

}; // end class delayed_thread 

void funcToRun()
{
	std::cout << "In funcToRun...\n";
	return;
}

int main()
{
	delayed_thread t(funcToRun);
	std::cout << "Working while thread 'suspended'...\n";
	t.trigger();
	std::cout << "Working while thread runs asynchronously...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "Done!\n";

}
