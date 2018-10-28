#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore {
public:

	//Default as locked.
	Semaphore(const size_t available = 0, const size_t capacity = -1) {
		_available = available;
		_capacity = capacity;
	}

	void notify() {
		std::lock_guard<decltype(_mutex)> lock(_mutex);
		++_available;
		if (_capacity > 0 && _available > _capacity) {
			_available = _capacity;
		}
		_condition.notify_one();
	}

	void wait() {
		std::unique_lock<decltype(_mutex)> lock(_mutex);
		//Handle fake wake-ups.
		while (_available == 0) {
			_condition.wait(lock);
		}
		--_available;
	}

	bool tryWait() {
		std::lock_guard<decltype(_mutex)> lock(_mutex);
		if (_available > 0) {
			--_available;
			return true;
		}
		return false;
	}

	void setAvailable(const size_t available) {
		std::unique_lock<decltype(_mutex)> lock(_mutex);
		_available = available;
	}

	void clear() {
		std::unique_lock<decltype(_mutex)> lock(_mutex);
		_available = 0;
	}

private:
	std::mutex _mutex;
	std::condition_variable _condition;
	size_t _available, _capacity;

};