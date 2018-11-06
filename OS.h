#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>
#include <xmmintrin.h> //_MM_SET_FLUSH_ZERO_MODE
#include "Error.h"

class OS {
public:

	static void setPriorityHigh() {
		setPriority(HIGH_PRIORITY_CLASS);
	}

	static void setPriority(const int value) {
		if (!SetPriorityClass(GetCurrentProcess(), value)) {
			throw Error("Failed to set process priority(%d)", GetLastError());
		}
	}

	static void minimizeWindow() {
		ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	}

	static void hideWindow() {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	static void showWindow() {
		ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
	}

	static void flushDenormalizedZero() {
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	}

	static const std::string getExePath() {
		char value[1024];
		int length = GetModuleFileNameA(NULL, value, sizeof(value));
		if (length == 0) {
			throw Error("Failed to get executable path: %d", GetLastError());
		}
		return std::string(value);
	}

	static const std::string getExeDirPath() {
		std::string exePath = getExePath();
		size_t i = exePath.rfind('\\');
		if (i != std::string::npos) {
			return exePath.substr(0, i + 1);
		}
		return exePath;
	}

	static const bool regValueExists(const HKEY hKey, const std::string &path, const std::string &name) {
		char value[1024];
		DWORD length;
		const LSTATUS status = RegGetValue(hKey, path.c_str(), name.c_str(), RRF_RT_ANY, NULL, value, &length);
		switch (status) {
		case ERROR_SUCCESS:
			return true;
		case ERROR_FILE_NOT_FOUND:
			return false;
		default:
			throw Error("Failed to check if register key value: %d", status);
		}
	}

	static const std::string regGetValue(const HKEY hKey, const std::string &path, const std::string &name) {
		char value[1024];
		DWORD length;
		const LSTATUS status = RegGetValue(hKey, path.c_str(), name.c_str(), RRF_RT_ANY, NULL, value, &length);
		if (status != ERROR_SUCCESS) {
			throw Error("Failed to get register value: %d", status);
		}
		return std::string(value);
	}

	static void regSetValue(const HKEY hKey, const std::string &path, const std::string &name, const std::string &value) {
		const LSTATUS status = RegSetKeyValueA(hKey, path.c_str(), name.c_str(), REG_SZ, value.c_str(), (DWORD)value.size());
		if (status != ERROR_SUCCESS) {
			throw Error("Failed to set register value: %d", status);
		}
	}

	static void regRemoveValue(const HKEY hKey, const std::string &path, const std::string &name) {
		const LSTATUS status = RegDeleteKeyValueA(hKey, path.c_str(), name.c_str());
		if (status != ERROR_SUCCESS) {
			throw Error("Failed to remove register value: %d", status);
		}
	}

};