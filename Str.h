#pragma once
#include <string>

class String {
public:

	String();
	String(const String &str);
	String(const std::string &str);
	String(const std::wstring &str);
	String(const char *str);
	String(const wchar_t *str);

	const bool startsWith(const String &str) const;
	const bool endsWith(const String &str) const;

	inline const size_t size() const {
		return _str.size();
	}

	inline const std::string str() const {
		return _str;
	}

	inline const std::string cStr() const {
		return _str.c_str();
	}

	inline const bool equals(const String &str) const {
		return _str.compare(str._str) == 0;
	}

	inline const int compare(const String &str) const {
		return _str.compare(str._str);
	}

	inline const bool contains(const String &str) const {
		return _str.find(str._str) != std::string::npos;
	}

	char& operator[](std::size_t index) {
		return _str[index];
	}

	const char& operator[](std::size_t index) const {
		return _str[index];
	}

private:
	std::string _str;

};