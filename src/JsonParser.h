#pragma once
#include "JsonNode.h"

class File;

class JsonParser {
public:

    static shared_ptr<JsonNode> fromString(const string& str);
    static shared_ptr<JsonNode> fromFile(const File& file);
    static void toFile(const File& file, const JsonNode& node);
    static const string toString(const JsonNode& node, const size_t depth = 1);

private:

    static shared_ptr<JsonNode> parseObject(const char*& p, size_t& row);
    static void parseField(const char*& p, JsonNode* pObjectNode, size_t& row);
    static string matchFieldID(const char*& p, size_t& row);

    static shared_ptr<JsonNode> parseArray(const char*& p, size_t& row);
    static void parseItem(const char*& p, JsonNode* pArrayNode, size_t& row);

    static shared_ptr<JsonNode> matchValue(const char*& p, size_t& row);
    static void match(const char*& str, const char expected, size_t& row);
    static void removeBlank(const char*& p, size_t& row);
    static string next(const char*& p, const char breakChar, size_t& row);
    static string next(const char*& p);
    static const bool isNumber(const string& s);
    static const bool isInteger(const string& s);
    static const string JsonParser::getPadding(const size_t depth);
    static const string formatDouble(const double value);

};