#include "Log.h"
#include <cstdarg>
#include <stdio.h>
#include <fstream> // ofstream
#include "Date.h"

using std::ofstream;
using std::ios_base;

#define BUFFER_SIZE 512

bool Log::_logToPrint = true;
bool Log::_logToFile = false;
char Log::_fileName[BUFFER_SIZE];

void Log::log(const char*const type, const char*const file, const unsigned int line, const char *const str, ...) {
	//Apply argument to user string.
	va_list ap;
	char userText[BUFFER_SIZE];
	va_start(ap, str);
	vsnprintf(userText, BUFFER_SIZE, str, ap);
	va_end(ap);

	//Create final string with logger specific format.
	char resultText[BUFFER_SIZE];
	snprintf(resultText, BUFFER_SIZE, "%s | %s | %s(%d) | %s\n", Date::toLocalDateTimeString().c_str(), type, file, line, userText);

	logText(resultText);
}

void Log::newLine() {
	logText("\n");
}

void Log::logText(const char * const text) {
	if (_logToPrint) {
		printf(text);
	}
	if (_logToFile) {
		ofstream outfile;
		outfile.open(_fileName, ios_base::app);
		outfile << text;
	}
}

void Log::clearFile() {
	ofstream outfile;
	outfile.open(_fileName);
	outfile << "";
}

void Log::logToPrint(const bool logToPrint) {
	_logToPrint = logToPrint;
}

void Log::logToFile(const string &fileName) {
	_logToFile = true;
	strcpy_s(_fileName, BUFFER_SIZE, fileName.c_str());
}