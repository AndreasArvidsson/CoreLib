#pragma once
#include <string>
#include <vector>

class File {
public:
    File();
    File(const File &file);
    File(const std::string &path);
    File(const char *path);
    File(const std::wstring &path);
    File(const wchar_t *path);

    const std::string getPath() const;
    const std::string getParentPath() const;
    const std::string getName() const;
    const std::string getShortName() const;
    const std::string getExtension() const;
    const bool getData(std::string &result) const;
	const bool getData(std::vector<std::string> &result) const;
	const size_t getData(char **result) const;
	const bool setData(const std::string &content) const;
    const bool exists() const;
	const time_t getLastModifiedTime() const;

private:
    std::string _path;

};