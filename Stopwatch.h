#pragma once
#include "Date.h"
#include <string>

class Stopwatch {
public:

	Stopwatch() {
		_name = "Timer";
		_t1 = 0;
		_index = _resetAt = _interval = 0;
	}

	Stopwatch(const std::string &name) {
		_name = name;
		_t1 = 0;
		_index = _resetAt = _interval = 0;
	}

	void start() {
		_t1 = Date::getCurrentTimeMillis();
		_index = _interval = 0;
	}

	void stop() {
		time_t duration = Date::getCurrentTimeMillis() - _t1;
		if (duration) {
			printf("%s: %lldms\n", _name.c_str(), Date::getCurrentTimeMillis() - _t1);
		}
		if (_interval) {
			printf("%s interval: %lldms\n", _name.c_str(), _interval);
		}
	}

	void resetAt(const size_t counts) {
		_resetAt = counts;
	}

	inline void tick() {
		++_index;
		if (_index >= _resetAt) {
			stop();
			start();
		}
	}

	inline void intervalStart() {
		_t1 = Date::getCurrentTimeMillis();
	}

	inline void intervalEnd() {
		_interval += Date::getCurrentTimeMillis() - _t1;
		if (_resetAt) {
			tick();
		}
	}

private:
	time_t _t1;
	std::string _name;
	size_t _resetAt;
	size_t _index;
	time_t _interval;

};