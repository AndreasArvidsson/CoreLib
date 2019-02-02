#include "Log.h"
#include <cstdarg>
#include <stdio.h>
#include <fstream> // std::ofstream
#include "Date.h"

#define BUFFER_SIZE 512

bool Log::_logToPrint = true;
bool Log::_logToFile = false;
std::string Log::_fileName = "log.txt";

void Log::log(const char*const type, const char*const file, const unsigned int line, const char *const str, ...) {
	//Apply argument to user string.
	va_list ap;
	char userText[BUFFER_SIZE];
	va_start(ap, str);
	vsnprintf(userText, BUFFER_SIZE, str, ap);
	va_end(ap);

	//Create final string with logger specific format.
	char resultText[BUFFER_SIZE];
	snprintf(resultText, BUFFER_SIZE, "%s | %s | %s(%d) | %s\n", Date::getLocalDateTimeString().c_str(), type, file, line, userText);

	if (_logToPrint) {
		printf(resultText);
	}
	if (_logToFile) {
		std::ofstream outfile;
		outfile.open(_fileName, std::ios_base::app);
		outfile << resultText;
	}
}

void Log::clearFile() {
	std::ofstream outfile;
	outfile.open(_fileName);
	outfile << "";
}

void Log::setFileName(const std::string &fileName) {
	_fileName = fileName;
}

void Log::logToPrint(const bool logToPrint) {
	_logToPrint = logToPrint;
}

void Log::logToFile(const bool logToFile) {
	_logToFile = logToFile;
}