#pragma once
#include "JsonNode.h"

class File;

class JsonParser {
public:

	static JsonNode* fromString(const std::string &str);
	static JsonNode* fromFile(const File &file);
	static void toFile(const File &file, const JsonNode &node);
	static const std::string toString(const JsonNode &node, const size_t depth = 1);

private:

	static JsonNode* parseObject(const char *&p, size_t &row);
	static void parseField(const char *&p, JsonNode *pObjectNode, size_t &row);
	static std::string matchFieldID(const char *&p, size_t &row);

	static JsonNode* parseArray(const char *&p, size_t &row);
	static void parseItem(const char *&p, JsonNode *pArrayNode, size_t &row);
	
	static JsonNode* matchValue(const char *&p, size_t &row);
	static void match(const char *&str, const char expected, size_t &row);
	static void removeBlank(const char *&p, size_t &row);
	static std::string next(const char *&p, const char breakChar, size_t &row);
	static std::string next(const char *&p);
	static const bool isNumber(const std::string &s);
	static const bool isInteger(const std::string &s);
	static const std::string JsonParser::getPadding(const size_t depth);
	static const std::string formatDouble(const double value);

};