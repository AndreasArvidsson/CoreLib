#include "Str.h"
#include <cstdarg>
#include <locale>

#define BUFFER_SIZE 512

// STATIC FUNCTIONS

std::string String::toUpperCase(const std::string &str) {
    const std::locale loc;
    std::string result(str.size(), char());
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = std::toupper(str[i], loc);
    }
    return result;
}

std::string String::toLowerCase(const std::string &str) {
    const std::locale loc;
    std::string result(str.size(), char());
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = std::tolower(str[i], loc);
    }
    return result;
}

std::string String::format(const char *const str, ...) {
    //Apply argument to user string.
    va_list ap;
    char buf[BUFFER_SIZE];
    va_start(ap, str);
    vsnprintf(buf, BUFFER_SIZE, str, ap);
    va_end(ap);
    return std::string(buf);
}

const bool String::equalsIgnoreCase(const std::string &str1, const std::string &str2) {
    if (str1.size() != str2.size()) {
        return false;
    }
    const std::locale loc;
    for (size_t i = 0; i < str1.size(); ++i) {
        if (std::tolower(str1[i], loc) != std::tolower(str2[i], loc)) {
            return false;
        }
    }
    return true;
}

// INSTANCE FUNCTION

String::String() { }

String::String(const String &str) : _str(str._str) { }

String::String(const std::string &str) : _str(str) { }

String::String(const std::wstring &str) {
	_str = std::string(str.begin(), str.end());
}

String::String(const char *str) {
	_str = str;
}

String::String(const wchar_t *str) {
	const std::wstring wstr(str);
	_str = std::string(wstr.begin(), wstr.end());
}

const bool String::startsWith(const String &str) const {
	if (str.size() > size()) {
		return false;
	}
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] != _str[i]) {
			return false;
		}
	}
	return true;
}

const bool String::endsWith(const String &str) const {
	if (str.size() > size()) {
		return false;
	}
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[str.size() - i - 1] != _str[size() - i - 1]) {
			return false;
		}
	}
	return true;
}
