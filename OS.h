#pragma once
#define NOMINMAX
#include <windows.h> //HKEY
#include <string>

namespace OS {

    void setPriorityHigh();
    void setPriority(const int value);
    void flushDenormalizedZero();

    void minimizeWindow();
    void hideWindow();
    void showWindow();
    void showInForeground();
    const bool isWindowMinimized();

    const std::string getExePath();
    const std::string getExeDirPath();

    const bool regValueExists(const HKEY hKey, const std::string &path, const std::string &name);
    const std::string regGetValue(const HKEY hKey, const std::string &path, const std::string &name);
    void regSetValue(const HKEY hKey, const std::string &path, const std::string &name, const std::string &value);
    void regRemoveValue(const HKEY hKey, const std::string &path, const std::string &name);

};