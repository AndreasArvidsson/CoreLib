#pragma once
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
#define LOG_INFO(str, ...)      Log::log("INFO ", __FILENAME__, __LINE__, str, ##__VA_ARGS__)
#define LOG_WARN(str, ...)      Log::log("WARN ", __FILENAME__, __LINE__, str, ##__VA_ARGS__)
#define LOG_ERROR(str, ...)     Log::log("ERROR", __FILENAME__, __LINE__, str, ##__VA_ARGS__)

class Log {
public:
	static void log(const char* type, const char* file, unsigned int line, const char *str, ...);
	static void clearFile();
	static void logToPrint(const bool logToPrint);
	static void logToFile(const std::string &fileName);

private:
	static bool _logToPrint;
	static bool _logToFile;
	static char _fileName[];

};