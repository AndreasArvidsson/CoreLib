#include "Str.h"
#include <cstdarg>
#include <locale>

using std::locale;

#define BUFFER_SIZE 512

// STATIC FUNCTIONS

string String::toUpperCase(const string &str) {
    const locale loc;
    string result(str.size(), char());
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = toupper(str[i], loc);
    }
    return result;
}

string String::toLowerCase(const string &str) {
    const locale loc;
    string result(str.size(), char());
    for (size_t i = 0; i < str.size(); ++i) {
        result[i] = tolower(str[i], loc);
    }
    return result;
}

string String::format(const char *const str, ...) {
    //Apply argument to user string.
    va_list ap;
    char buf[BUFFER_SIZE];
    va_start(ap, str);
    vsnprintf(buf, BUFFER_SIZE, str, ap);
    va_end(ap);
    return string(buf);
}

string String::toString(const double value) {
    char buf[BUFFER_SIZE];
    const int size = snprintf(buf, BUFFER_SIZE, "%f", value);
    int i = size - 1;
    while (buf[i] == '0' || buf[i] == '.') {
        if (buf[i] == '.') {
            buf[i] = '\0';
            break;
        }
        buf[i] = '\0';
        --i;
    }
    return string(buf);
}

string String::toString(const wstring& wstring) {
    string res = "";
    for (const wchar_t c : wstring) {
        res += (char)c;
    }
    return res;
}

const bool String::equalsIgnoreCase(const string &str1, const string &str2) {
    if (str1.size() != str2.size()) {
        return false;
    }
    const locale loc;
    for (size_t i = 0; i < str1.size(); ++i) {
        if (tolower(str1[i], loc) != tolower(str2[i], loc)) {
            return false;
        }
    }
    return true;
}

// INSTANCE FUNCTION

String::String() { }

String::String(const String &str) : _str(str._str) { }

String::String(const string &str) : _str(str) { }

String::String(const wstring &str) {
    _str = "";
    for (const wchar_t c : str) {
        _str += (char)c;
    }
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
