#pragma once
#include <atomic>

class SpinLock {
public:

    inline void lock() {
        while (_flag.test_and_set());
    }

    inline void unlock() {
        _flag.clear();
    }

private:
    std::atomic_flag _flag;

};