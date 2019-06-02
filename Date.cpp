#include "Date.h"
#include <cstring> //strstr
#include <cmath> //pow
#define NOMINMAX
#include "windows.h"
#include "Timezoneapi.h" //GetTimeZoneInformation

#define BUFFER_SIZE 25

intptr_t ntdll = _loaddll((char*)"ntdll.dll");
NtDelayExecution_t *Date::NtDelayExecution = (NtDelayExecution_t *)_getdllprocaddr(ntdll, (char*)"NtDelayExecution", -1);

const std::string Date::getLocalDateTimeString() {
	return getLocalDateTimeString(getCurrentTimeMillis());
}

const std::string Date::getLocalDateTimeString(const time_t timestamp) {
	//strftime can only handle seconds.
	time_t tSec = timestamp / 1000 - getTimezoneOffset();
	char buf[BUFFER_SIZE];
	struct tm *pTM = gmtime(&tSec);
	strftime(buf, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", pTM);
	return std::string(buf);
}

const std::string Date::getIsoString() {
	return getIsoString(getCurrentTimeMillis());
}

const std::string Date::getIsoString(const time_t timestamp) {
	//strftime can only handle seconds.
	time_t tSec = timestamp / 1000;
	char buf[BUFFER_SIZE];
	struct tm *pTM = gmtime(&tSec);
	int fractions = tSec ? (int)(timestamp % tSec) : 0;
	size_t size = strftime(buf, BUFFER_SIZE, "%Y-%m-%dT%H:%M:%S", pTM);
	if (fractions) {
		sprintf(buf + size, ".%03dZ", fractions);
	}
	else {
		sprintf(buf + size, "Z");
	}
	return std::string(buf);
}

const bool Date::fromIsoString(const char *isoString, time_t *timestampOut) {
	int yyyy, MM, dd, hh, mm, ss, f;
	yyyy = MM = dd = -1;
	hh = mm = ss = f = 0;
	sscanf(isoString, "%04d-%02d-%02dT%02d:%02d:%02d.%03d", &yyyy, &MM, &dd, &hh, &mm, &ss, &f);
	if (yyyy < 1 || MM < 1 || dd < 1) {
		return false;
	}
	struct tm tm;
	tm.tm_isdst = 0;
	tm.tm_wday = 0;
	tm.tm_yday = 0;
	tm.tm_year = yyyy - 1900;
	tm.tm_mon = MM - 1;
	tm.tm_mday = dd;
	tm.tm_hour = hh;
	tm.tm_min = mm;
	tm.tm_sec = ss;
	time_t timesec = mktime(&tm);
	if (timesec < 0) {
		return false;
	}
	if (f) {
		const char *ptr = std::strstr(isoString, ".") + 1;
		int i;
		for (i = 0; i < 3; ++i) {
			if (ptr[i] < '0' || ptr[i] > '9') {
				f *= (int)std::pow(10, 3 - i);
				break;
			}
		}
	}

	_tzset();
	*timestampOut = 1000 * (timesec - _timezone) + f;
	return true;
}

const long Date::getTimezoneOffset() {
	TIME_ZONE_INFORMATION timeZoneInformation;
	DWORD res = GetTimeZoneInformation(&timeZoneInformation);

	//Bias / offset in minutes
	long bias = timeZoneInformation.Bias;
	switch (res) {
	case TIME_ZONE_ID_STANDARD:
		bias += timeZoneInformation.StandardBias;
		break;
	case TIME_ZONE_ID_DAYLIGHT:
		bias += timeZoneInformation.DaylightBias;
		break;
	}
	//Convert to seconds
	return bias * 60;
}