#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore {
public:

	//Default as locked.
    Semaphore(const size_t available = 0, const size_t capacity = -1);

	void notify();
	void wait();
	bool tryWait();

	void setAvailable(const size_t available);
	void setCapacity(const size_t capacity);
	void clear();

private:
	std::mutex _mutex;
	std::condition_variable _condition;
	size_t _available, _capacity;

};