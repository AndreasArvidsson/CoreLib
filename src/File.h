#pragma once
#include <string>
#include <vector>
#include <memory>

using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;

class File {
public:
    File();
    File(const File &file);
    File(const string &path);
    File(const char *path);
    File(const wstring &path);
    File(const wchar_t *path);

    const string getPath() const;
    const string getParentPath() const;
    const string getName() const;
    const string getShortName() const;
    const string getExtension() const;
    const bool getData(string &result) const;
	const bool getData(vector<string> &result) const;
    const size_t getData(unique_ptr<char[]> *result) const;
	const bool setData(const string &content) const;
    const bool exists() const;
	const time_t getLastModifiedTime() const;

private:
    string _path;

};