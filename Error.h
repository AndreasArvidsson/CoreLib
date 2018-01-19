#pragma once
#include <exception>
#include <cstdarg>		//va_start, va_end
#include <cstdio>		//vsnprintf

class Error : public std::exception {
public:

	Error(char const* fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(text, sizeof text, fmt, ap);
		va_end(ap);
	}

	char const* what() const throw() override {
		return text;
	}

private:
	char text[1000];

};