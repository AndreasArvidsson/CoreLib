#pragma once
#include <windows.h>
#include "Error.h"
#include <xmmintrin.h>

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

	static void minimize() {
		ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	}

	static void show() {
		ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
	}

	static void flushDenormalizedZero() {
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
	}

};