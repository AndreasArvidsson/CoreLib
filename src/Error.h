#pragma once
#include <exception>
#include <cstdarg>		//va_start, va_end
#include <cstdio>		//vsnprintf
#include <string.h>		//strcpy_s

class Error : public std::exception {
public:

	Error() {
		strcpy_s(_text, "Unknown error");
	}

	Error(char const* fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(_text, sizeof _text, fmt, ap);
		va_end(ap);
	}

	char const* what() const throw() override {
		return _text;
	}

private:
	char _text[1000];

};