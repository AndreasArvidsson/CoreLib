#include "File.h"
#include <fstream> //ofstream 
#include "Str.h"

#ifdef _WIN32
#include <filesystem>
namespace fs = std::experimental::filesystem::v1;
#else
#include <sys/stat.h>
#endif

File::File() {
}

File::File(const File &file) {
	_path = file._path;
}

File::File(const std::string &path) {
	_path = path;
}

File::File(const char *path) {
	_path = path;
}

File::File(const std::wstring &path) {
	_path = String::toString(path);
}

File::File(const wchar_t *path) {
	_path = String::toString(path);
}

const std::string File::getPath() const {
	return _path;
}

const std::string File::getParentPath() const {
	return _path.substr(0, _path.find_last_of("/\\"));
}

const std::string File::getName() const {
	return _path.substr(_path.find_last_of("/\\") + 1);
}

const std::string File::getShortName() const {
	size_t index = _path.find_last_of("/\\") + 1;
	return _path.substr(index, _path.find_last_of(".") - index);
}

const std::string File::getExtension() const {
	return _path.substr(_path.find_last_of(".") + 1);
}

const bool File::getData(std::string &result) const {
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

const bool File::getData(std::vector<std::string> &result) const {
	FILE *pFile = fopen(_path.c_str(), "r");
	if (!pFile) {
		return false;
	}
	char c;
	std::string row;
	while ((c = (char)fgetc(pFile)) != EOF) {
		if (c == '\n') {
			result.push_back(row);
			row = std::string();
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

const size_t File::getData(char **result) const {
	std::streampos size;
	std::ifstream file(_path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())  {
		size = file.tellg();
		*result = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(*result, size);
		file.close();
		return size;
	}
	return 0;
}

const bool File::setData(const std::string &content) const {
	std::ofstream file(_path.c_str());
	if (file.is_open()) {
		file << content.c_str();
		file.close();
		return true;
	}
	return false;
}

const bool File::exists() const {
#ifdef _WIN32
	return fs::exists(_path);
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