#include "OS.h"
#include <xmmintrin.h> //_MM_SET_FLUSH_ZERO_MODE
#include "Error.h"

void OS::setPriorityHigh() {
    setPriority(HIGH_PRIORITY_CLASS);
}

void OS::setPriority(const int value) {
    if (!SetPriorityClass(GetCurrentProcess(), value)) {
        throw Error("Failed to set process priority(%d)", GetLastError());
    }
}

void OS::flushDenormalizedZero() {
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
}

void OS::minimizeWindow() {
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
}

void OS::hideWindow() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void OS::showWindow() {
    ShowWindow(GetConsoleWindow(), SW_RESTORE);
}

void OS::showInForeground() {
    SetForegroundWindow(GetConsoleWindow());
}

const bool OS::isWindowMinimized() {
    return IsIconic(GetConsoleWindow());
}

const std::string OS::getExePath() {
    char value[1024];
    int length = GetModuleFileNameA(NULL, value, sizeof(value));
    if (length == 0) {
        throw Error("Failed to get executable path: %d", GetLastError());
    }
    return std::string(value);
}

const std::string OS::getExeDirPath() {
    std::string exePath = getExePath();
    size_t i = exePath.rfind('\\');
    if (i != std::string::npos) {
        return exePath.substr(0, i + 1);
    }
    return exePath;
}

const bool OS::regValueExists(const HKEY hKey, const std::string &path, const std::string &name) {
    const LSTATUS status = RegGetValue(hKey, path.c_str(), name.c_str(), RRF_RT_ANY, NULL, nullptr, nullptr);
    switch (status) {
    case ERROR_SUCCESS:
        return true;
    case ERROR_FILE_NOT_FOUND:
        return false;
    default:
        throw Error("Failed to check if register key value: %d", status);
    }
}

const std::string OS::regGetValue(const HKEY hKey, const std::string &path, const std::string &name) {
    char value[1024];
    DWORD length;
    const LSTATUS status = RegGetValue(hKey, path.c_str(), name.c_str(), RRF_RT_ANY, NULL, value, &length);
    if (status != ERROR_SUCCESS) {
        throw Error("Failed to get register value: %d", status);
    }
    return std::string(value);
}

void OS::regSetValue(const HKEY hKey, const std::string &path, const std::string &name, const std::string &value) {
    const LSTATUS status = RegSetKeyValueA(hKey, path.c_str(), name.c_str(), REG_SZ, value.c_str(), (DWORD)value.size());
    if (status != ERROR_SUCCESS) {
        throw Error("Failed to set register value: %d", status);
    }
}

void OS::regRemoveValue(const HKEY hKey, const std::string &path, const std::string &name) {
    const LSTATUS status = RegDeleteKeyValueA(hKey, path.c_str(), name.c_str());
    if (status != ERROR_SUCCESS) {
        throw Error("Failed to remove register value: %d", status);
    }
}

const std::string OS::getLastError() {
    //Get the error message, if any.
    const DWORD errorMessageID = ::GetLastError();

    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}