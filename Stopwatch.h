#pragma once
#include <string>
#include <chrono>

#undef max // to get duration::max() to work
#define BUFFER_SIZE 128

class Stopwatch {
public:

	Stopwatch(const size_t resetAt = 0) {
		init("Timer", resetAt);
	}

	Stopwatch(const std::string &name, const size_t resetAt = 0) {
		init(name, resetAt);
	}

	inline void start() {
		_t1 = std::chrono::high_resolution_clock::now();
		_index = 0;
	}

	inline void stop() {
		const std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - _t1;
		printf("%s | %.2fms\n", _name, duration.count());
	}

	inline void intervalStart() {
		_t1 = std::chrono::high_resolution_clock::now();
	}

	inline void intervalEnd() {
		const std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - _t1;
		_interval += duration;
		if (duration < _intervalMin) {
			_intervalMin = duration;
		}
		if (duration > _intervalMax) {
			_intervalMax = duration;
		}
		if (_resetAt) {
			++_index;
			if (_index >= _resetAt) {
				stopInterval();
			}
		}
	}

	inline void stopInterval() {
		if (_index) {
			printf("%s interval | Total: %.0fms, Min: %.2fms, Max: %.2fms, Mean: %.2fms\n", _name, _interval.count(), _intervalMin.count(), _intervalMax.count(), (_interval / _index).count());
		}
		else {
			printf("%s interval | Total: %.0fms, Min: %.2fms, Max: %.2fms\n", _name, _interval.count(), _intervalMin.count(), _intervalMax.count());
		}
		reset();
	}

	inline void resetAt(const size_t counts) {
		_resetAt = counts;
	}

	inline void tick() {
		++_index;
		if (_resetAt && _index >= _resetAt) {
			stop();
			start();
		}
	}

private:
	char _name[BUFFER_SIZE];
	std::chrono::high_resolution_clock::time_point _t1;
	std::chrono::duration<double, std::milli> _interval, _intervalMin, _intervalMax;
	size_t _resetAt;
	size_t _index;
	std::chrono::milliseconds _tmp;

	void init(const std::string &name, const size_t resetAt) {
		strcpy_s(_name, BUFFER_SIZE, name.c_str());
		_resetAt = resetAt;
		reset();
	}

	inline void reset() {
		_intervalMin = { std::chrono::duration_values<double>::max() };
		_interval = _intervalMax = std::chrono::duration_values<std::chrono::milliseconds>::zero();
		_index = 0;
		//_t1 = 0;
	}

};