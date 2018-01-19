#pragma once
#include <string>
#include <chrono>
#include <thread>

class Date {
public:

	static inline const time_t Date::getCurrentTimeMillis() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	static inline void sleepMillis(const time_t ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	static const std::string toIsoString(const time_t timestamp);
	static const bool fromIsoString(const char *isoString, time_t *timestampOut);

};