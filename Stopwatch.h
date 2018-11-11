#pragma once
#include <string>
#include <chrono>

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
		printf("%s | %.2fms\n", _name.c_str(), duration.count());
	}

	inline void intervalStart() {
		_t1 = std::chrono::high_resolution_clock::now();
	}

	inline void intervalEnd() {
		const std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - _t1;
		_interval += duration;
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
			printf("%s interval | Total: %.0fms, Max: %.2fms, Mean: %.2fms\n", _name.c_str(), _interval.count(), _intervalMax.count(), (_interval / _index).count());
		}
		else {
			printf("%s interval | Total: %.0fms, Max: %.2fms,\n", _name.c_str(), _interval.count(), _intervalMax.count());
		}
		_interval = _intervalMax = std::chrono::duration_values<std::chrono::milliseconds>::zero();
		_index = 0;
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
	std::string _name;
	std::chrono::high_resolution_clock::time_point _t1;
	std::chrono::duration<double, std::milli> _interval, _intervalMax;
	size_t _resetAt;
	size_t _index;

	void init(const std::string &name, const size_t resetAt) {
		_name = name;
		_resetAt = resetAt;
		_interval = _intervalMax = std::chrono::duration_values<std::chrono::milliseconds>::zero();
		_index = 0;
		//_t1 = 0;
	}

};

//#pragma once
//#include "Date.h"
//#include <string>
//
//#include <chrono>
//
//class Stopwatch {
//public:
//
//	Stopwatch(const size_t resetAt = 0) {
//		_name = "Timer";
//		_resetAt = resetAt;
//		_t1 = 0;
//		_index = _interval = 0;
//	}
//
//	Stopwatch(const std::string &name, const size_t resetAt = 0) {
//		_name = name;
//		_resetAt = resetAt;
//		_t1 = 0;
//		_index = _interval = 0;
//	}
//
//	inline void start() {
//		_t1 = Date::getCurrentTimeMillis();
//		_index = 0;
//	}
//
//	inline void stop() {
//		time_t duration = Date::getCurrentTimeMillis() - _t1;
//		printf("%s: %lldms\n", _name.c_str(), Date::getCurrentTimeMillis() - _t1);
//	}
//
//	inline void stopInterval() {
//		if (_index) {
//			printf("%s interval: %lldms, mean: %.2fms\n", _name.c_str(), _interval, (float)_interval / _index);
//		}
//		else {
//			printf("%s interval: %lldms\n", _name.c_str(), _interval);
//		}
//		_interval = 0;
//		_index = 0;
//	}
//
//	inline void resetAt(const size_t counts) {
//		_resetAt = counts;
//	}
//
//	inline void tick() {
//		++_index;
//		if (_resetAt && _index >= _resetAt) {
//			stop();
//			start();
//		}
//	}
//
//	inline void intervalStart() {
//		_t1 = Date::getCurrentTimeMillis();
//	}
//
//	inline void intervalEnd() {
//		_interval += Date::getCurrentTimeMillis() - _t1;
//		++_index;
//		if (_resetAt && _index >= _resetAt) {
//			stopInterval();
//		}
//	}
//
//private:
//	time_t _t1;
//	std::string _name;
//	size_t _resetAt;
//	size_t _index;
//	time_t _interval;
//
//};