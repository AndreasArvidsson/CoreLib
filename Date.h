#pragma once
#include <string>
#include <chrono>
#include <thread>

//NtDelayExecution_t
extern "C" {
    typedef long long intptr_t;
    typedef union _LARGE_INTEGER *PLARGE_INTEGER;
    typedef long NTSTATUS;
    typedef NTSTATUS __stdcall NtDelayExecution_t(unsigned char Alertable, PLARGE_INTEGER Interval);
}

class Date {
public:

	static inline const time_t getCurrentTimeMillis() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
    static inline void sleepMillis(const time_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

	static inline void sleepMilli() {
        static long long delay = 1 * -(1000 / 100) /* relative 100-ns intervals */;
        NtDelayExecution(0, (PLARGE_INTEGER)&delay);
	}

	static inline void sleepSeconds(const time_t s) {
		std::this_thread::sleep_for(std::chrono::seconds(s));
	}

	static const std::string getLocalDateTimeString();
	static const std::string getLocalDateTimeString(const time_t timestamp);
	static const std::string getIsoString();
	static const std::string getIsoString(const time_t timestamp);
	static const bool fromIsoString(const char *isoString, time_t *timestampOut);

private:
	static const long getTimezoneOffset();
    static NtDelayExecution_t *NtDelayExecution;

};