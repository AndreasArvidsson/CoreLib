#pragma once
#include <string>
#include <chrono>
#include <thread>

class Date {
public:

	static inline const time_t Date::getCurrentTimeMillis() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	static inline void sleepMicros(const time_t us) {
		std::this_thread::sleep_for(std::chrono::microseconds(us));
	}

	static inline void sleepMillis(const time_t ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	static inline void sleepSeconds(const time_t s) {
		std::this_thread::sleep_for(std::chrono::seconds(s));
	}

	static const std::string getLocalDateTimeString();
	static const std::string getLocalDateTimeString(const time_t timestamp);
	static const std::string getIsoString();
	static const std::string getIsoString(const time_t timestamp);
	static const bool fromIsoString(const char *isoString, time_t *timestampOut);

};