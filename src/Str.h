#pragma once
#include <string>

using std::string;
using std::wstring;

class String {
public:

    static string toUpperCase(const string &str);
    static string toLowerCase(const string &str);
    static string format(const char *const str, ...);
    static string toString(const wstring& str);
    static const bool equalsIgnoreCase(const string &str1, const string &str2);

	String();
	String(const String &str);
	String(const string &str);
	String(const wstring &str);

	const bool startsWith(const String &str) const;
	const bool endsWith(const String &str) const;

	inline const size_t size() const {
		return _str.size();
	}

	inline const string str() const {
		return _str;
	}

	inline const string cStr() const {
		return _str.c_str();
	}

	inline const bool equals(const String &str) const {
		return _str.compare(str._str) == 0;
	}

	inline const int compare(const String &str) const {
		return _str.compare(str._str);
	}

	inline const bool contains(const String &str) const {
		return _str.find(str._str) != string::npos;
	}

	char& operator[](size_t index) {
		return _str[index];
	}

	const char& operator[](size_t index) const {
		return _str[index];
	}

private:
	string _str;

};