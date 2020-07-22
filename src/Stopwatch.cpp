#include "Stopwatch.h"

Stopwatch::Stopwatch(const size_t resetAt) {
    init("Timer", resetAt);
}

Stopwatch::Stopwatch(const string &name, const size_t resetAt) {
    init(name, resetAt);
}

void Stopwatch::init(const string &name, const size_t resetAt) {
    strcpy_s(_name, BUFFER_SIZE, name.c_str());
    _resetAt = resetAt;
    reset();
}