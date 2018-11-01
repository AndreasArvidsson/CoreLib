#pragma once
#include "Date.h"
#include <string>

class Stopwatch {
public:

	Stopwatch(const size_t resetAt = 0) {
		_name = "Timer";
		_resetAt = resetAt;
		_t1 = 0;
		_index = _interval = 0;
	}

	Stopwatch(const std::string &name, const size_t resetAt = 0) {
		_name = name;
		_resetAt = resetAt;
		_t1 = 0;
		_index = _interval = 0;
	}

	inline void start() {
		_t1 = Date::getCurrentTimeMillis();
		_index = 0;
	}

	inline void stop() {
		time_t duration = Date::getCurrentTimeMillis() - _t1;
		printf("%s: %lldms\n", _name.c_str(), Date::getCurrentTimeMillis() - _t1);
	}

	inline void stopInterval() {
		if (_index) {
			printf("%s interval: %lldms, mean: %.2fms\n", _name.c_str(), _interval, (float)_interval / _index);
		}
		else {
			printf("%s interval: %lldms\n", _name.c_str(), _interval);
		}
		_interval = 0;
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

	inline void intervalStart() {
		_t1 = Date::getCurrentTimeMillis();
	}

	inline void intervalEnd() {
		_interval += Date::getCurrentTimeMillis() - _t1;
		++_index;
		if (_resetAt && _index >= _resetAt) {
			stopInterval();
		}
	}

private:
	time_t _t1;
	std::string _name;
	size_t _resetAt;
	size_t _index;
	time_t _interval;

};