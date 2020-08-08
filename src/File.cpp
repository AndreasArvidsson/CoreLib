#include "File.h"
#include <fstream> //ofstream 
#include "Str.h"

#ifdef _WIN32
#include <filesystem>
#else
#include <sys/stat.h>
#endif

using std::streampos;
using std::ifstream;
using std::make_unique;
using std::ofstream;
using std::ios;

File::File() {
}

File::File(const File &file) {
	_path = file._path;
}

File::File(const string &path) {
	_path = path;
}

File::File(const char *path) {
	_path = path;
}

File::File(const wstring &path) {
	_path = String::toString(path);
}

File::File(const wchar_t *path) {
	_path = String::toString(path);
}

const string File::getPath() const {
	return _path;
}

const string File::getParentPath() const {
	return _path.substr(0, _path.find_last_of("/\\"));
}

const string File::getName() const {
	return _path.substr(_path.find_last_of("/\\") + 1);
}

const string File::getShortName() const {
	size_t index = _path.find_last_of("/\\") + 1;
	return _path.substr(index, _path.find_last_of(".") - index);
}

const string File::getExtension() const {
	return _path.substr(_path.find_last_of(".") + 1);
}

const bool File::getData(string &result) const {
	FILE *pFile = fopen(_path.c_str(), "r");
	if (!pFile) {
		return false;
	}
	result = "";
	char c;
	while ((c = (char)fgetc(pFile)) != EOF) {
		result += c;
	}
	fclose(pFile);
	return true;
}

const bool File::getData(vector<string> &result) const {
	FILE *pFile = fopen(_path.c_str(), "r");
	if (!pFile) {
		return false;
	}
	char c;
	string row;
	while ((c = (char)fgetc(pFile)) != EOF) {
		if (c == '\n') {
			result.push_back(row);
			row = string();
		}
		else {
			row += c;
		}
	}
	if (row.size()) {
		result.push_back(row);
	}
	return true;
}

const size_t File::getData(unique_ptr<char[]>* result) const {
	size_t size;
	ifstream file(_path.c_str(), ios::in | ios::binary | ios::ate);
	if (file.is_open()) {
		size = file.tellg();
		if (size) {
			*result = make_unique<char[]>(size + 1);
			file.seekg(0, ios::beg);
			file.read(result->get(), size);
			result->get()[size] = '\0';
		}
		file.close();
		return size;
	}
	return 0;
}

const bool File::setData(const string &content) const {
	ofstream file(_path.c_str());
	if (file.is_open()) {
		file << content.c_str();
		file.close();
		return true;
	}
	return false;
}

const bool File::exists() const {
#ifdef _WIN32
	return  std::filesystem::exists(_path);
#else
	struct stat buffer;
	return (stat(_path.c_str(), &buffer) == 0);
#endif
}

const time_t File::getLastModifiedTime() const {
	struct stat result;
	if (stat(_path.c_str(), &result) == 0) {
		return result.st_mtime;
	}
	return -1;
}