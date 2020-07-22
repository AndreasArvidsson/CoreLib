#include "Semaphore.h"

using std::lock;
using std::lock_guard;
using std::unique_lock;

Semaphore::Semaphore(const size_t available, const size_t capacity) {
    _available = available;
    _capacity = capacity;
}

void Semaphore::notify() {
    lock_guard<decltype(_mutex)> lock(_mutex);
    ++_available;
    if (_capacity > 0 && _available > _capacity) {
        _available = _capacity;
    }
    _condition.notify_one();
}

void Semaphore::wait() {
    unique_lock<decltype(_mutex)> lock(_mutex);
    //Handle fake wake-ups.
    while (_available == 0) {
        _condition.wait(lock);
    }
    --_available;
}

bool Semaphore::tryWait() {
    lock_guard<decltype(_mutex)> lock(_mutex);
    if (_available > 0) {
        --_available;
        return true;
    }
    return false;
}

void Semaphore::setAvailable(const size_t available) {
    unique_lock<decltype(_mutex)> lock(_mutex);
    _available = available;
}

void Semaphore::setCapacity(const size_t capacity) {
    unique_lock<decltype(_mutex)> lock(_mutex);
    _capacity = capacity;
}

void Semaphore::clear() {
    unique_lock<decltype(_mutex)> lock(_mutex);
    _available = 0;
}