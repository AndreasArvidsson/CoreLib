#pragma once
#define NOMINMAX
#include <windows.h> //HKEY
#include <string>

using std::string;

namespace OS {

    void setPriorityHigh();
    void setPriority(const int value);
    void flushDenormalizedZero();

    void minimizeWindow();
    void hideWindow();
    void showWindow();
    void showInForeground();
    const bool isWindowMinimized();

    const string getExePath();
    const string getExeDirPath();
    const string getLastError();

    const bool regValueExists(const HKEY hKey, const string &path, const string &name);
    const string regGetValue(const HKEY hKey, const string &path, const string &name);
    void regSetValue(const HKEY hKey, const string &path, const string &name, const string &value);
    void regRemoveValue(const HKEY hKey, const string &path, const string &name);

};