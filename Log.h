#pragma once
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define __LOG_INFO__(str, ...)  Log::log("INFO ", __FILENAME__, __LINE__, str, ##__VA_ARGS__)
#define __LOG_WARN__(str, ...)  Log::log("WARN ", __FILENAME__, __LINE__, str, ##__VA_ARGS__)
#define __LOG_ERROR__(str, ...) Log::log("ERROR", __FILENAME__, __LINE__, str, ##__VA_ARGS__)
#define __LOG_NL__()            Log::newLine();

#ifndef LOG_INFO
#define LOG_INFO				__LOG_INFO__
#define LOG_WARN				__LOG_WARN__
#define LOG_ERROR				__LOG_ERROR__
#define LOG_NL					__LOG_NL__
#endif

class Log {
public:
	static void log(const char* type, const char* file, unsigned int line, const char *str, ...);
	static void newLine();
	static void clearFile();
	static void logToPrint(const bool logToPrint);
	static void logToFile(const std::string &fileName);

private:
	static bool _logToPrint;
	static bool _logToFile;
	static char _fileName[];

	static void logText(const char * const text);

};