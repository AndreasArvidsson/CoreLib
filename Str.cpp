#include "Str.h"

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
