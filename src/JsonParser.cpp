#include "JsonParser.h"
#include <cctype> //isdigit
#include "File.h"
#include "Error.h"

using std::make_shared;
using std::to_string;

shared_ptr<JsonNode> JsonParser::fromString(const string& str) {
    const char* p = str.c_str();
    size_t row = 1;
    removeBlank(p, row);
    return parseObject(p, row);
}

shared_ptr<JsonNode> JsonParser::fromFile(const File& file) {
    string content;
    if (!file.getData(content)) {
        throw Error("JSON - Can't read file '%s'", file.getPath().c_str());
    }
    return fromString(content);
}

void JsonParser::toFile(const File& file, const JsonNode& node) {
    if (!file.setData(JsonParser::toString(node))) {
        throw Error("JSON - Cant write content to file '%s'", file.getPath().c_str());
    }
}

shared_ptr<JsonNode> JsonParser::parseObject(const char*& p, size_t& row) {
    shared_ptr<JsonNode> pObjectNode = make_shared<JsonNode>(JsonNodeType::OBJECT);
    match(p, '{', row);
    removeBlank(p, row);
    while (*p != '}' && *p != '\0') {
        parseField(p, pObjectNode.get(), row);
    }
    match(p, '}', row);
    return pObjectNode;
}

void JsonParser::parseField(const char*& p, JsonNode* pObjectNode, size_t& row) {
    string name = matchFieldID(p, row);
    if (pObjectNode->has(name)) {
        throw Error("JSON(%lu) - Duplicate field name '%s'", row, name.c_str());
    }
    removeBlank(p, row);
    match(p, ':', row);
    removeBlank(p, row);
    pObjectNode->put(name, matchValue(p, row));
    removeBlank(p, row);
    if (*p == ',') {
        match(p, ',', row);
        removeBlank(p, row);
        parseField(p, pObjectNode, row);
    }
}

shared_ptr<JsonNode> JsonParser::parseArray(const char*& p, size_t& row) {
    shared_ptr<JsonNode> pArrayNode = make_shared<JsonNode>(JsonNodeType::ARRAY);
    match(p, '[', row);
    removeBlank(p, row);
    while (*p != ']' && *p != '\0') {
        parseItem(p, pArrayNode.get(), row);
    }
    match(p, ']', row);
    return pArrayNode;
}

void JsonParser::parseItem(const char*& p, JsonNode* pArrayNode, size_t& row) {
    pArrayNode->add(matchValue(p, row));
    removeBlank(p, row);
    if (*p == ',') {
        match(p, ',', row);
        removeBlank(p, row);
        parseItem(p, pArrayNode, row);
    }
}

string JsonParser::matchFieldID(const char*& p, size_t& row) {
    match(p, '"', row);
    string name = next(p, '"', row);
    if (name.size() == 0) {
        throw Error("JSON(%lu) - Expected ID near '%s'", row, next(p, '\n', row).c_str());
    }
    match(p, '"', row);
    return name;
}

shared_ptr<JsonNode> JsonParser::matchValue(const char*& p, size_t& row) {
    if (*p == '{') {
        return parseObject(p, row);
    }
    if (*p == '[') {
        return parseArray(p, row);
    }
    if (*p == '"') {
        match(p, '"', row);
        string value = next(p, '"', row);
        shared_ptr<JsonNode> pNode = make_shared<JsonNode>(value);
        match(p, '"', row);
        return pNode;
    }
    string tmp = next(p).c_str();
    if (strcmp(tmp.c_str(), "true") == 0) {
        return make_shared<JsonNode>(true);
    }
    else if (strcmp(tmp.c_str(), "false") == 0) {
        return make_shared<JsonNode>(false);
    }
    else if (isNumber(tmp)) {
        if (isInteger(tmp)) {
            return make_shared<JsonNode>(atoi(tmp.c_str()));
        }
        else {
            return make_shared<JsonNode>(atof(tmp.c_str()));
        }
    }
    throw Error("JSON(%lu) - Unknown value '%s' near '%s'", row, tmp.c_str(), next(p, '\n', row).c_str());
}

void JsonParser::match(const char*& str, const char expected, size_t& row) {
    if (*str != expected) {
        throw Error("JSON(%lu) - Expected '%c', found '%s'", row, expected, next(str, '\n', row).c_str());
    }
    ++str; //Step over the char.
}

void JsonParser::removeBlank(const char*& p, size_t& row) {
    while (*p == ' ' || *p == '\n' || *p == '\t') {
        if (*p == '\n') {
            ++row;
        }
        ++p;
    }
}

string JsonParser::next(const char*& p, const char breakChar, size_t& row) {
    const char* begin = p;
    while (*p != breakChar && *p != '\0') {
        if (*p == '\n') {
            ++row;
        }
        ++p;
    }
    return string(begin, p);
}

string JsonParser::next(const char*& p) {
    const char* begin = p;
    while (isalnum(*p) || *p == '.' || *p == '-') {
        ++p;
    }
    return string(begin, p);
}

const bool JsonParser::isNumber(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && (isdigit(*it) || *it == '-' || *it == '.')) {
        ++it;
    }
    return !s.empty() && it == s.end();
}

const bool JsonParser::isInteger(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && (isdigit(*it) || *it == '-')) {
        ++it;
    }
    return !s.empty() && it == s.end();
}

const string JsonParser::toString(const JsonNode& node, const size_t depth) {
    string padding = getPadding(depth);
    switch (node.getNodeType()) {
    case JsonNodeType::NUMBER:
        return formatDouble(node.doubleValue());
    case JsonNodeType::INTEGER:
        return to_string(node.intValue());
    case JsonNodeType::TEXT:
        return "\"" + node.textValue() + "\"";
    case JsonNodeType::BOOL:
        return string(node.boolValue() ? "true" : "false");
    case JsonNodeType::NULL_:
        return string("null");
    case JsonNodeType::OBJECT: {
        string out = "{";
        bool addComma = false;
        for (const string& fieldName : node.getOrder()) {
            const shared_ptr<JsonNode> pField = node.get(fieldName);
            if (addComma) {
                out += ",";
            }
            else {
                addComma = true;
            }
            out += "\n" + padding + "\"" + fieldName + "\" : " + toString(*pField, depth + 1);
        }
        out += "\n" + getPadding(depth - 1) + "}";
        return out;
    }
    case JsonNodeType::ARRAY: {
        string out = "[";
        for (size_t i = 0; i < node.size(); ++i) {
            out += "\n" + padding + toString(*node.get(i), depth + 1);
            if (i + 1 < node.size()) {
                out += ",";
            }
        }
        out += "\n" + getPadding(depth - 1) + "]";
        return out;
    }
    default:
        return padding + string("ERROR!!!");
    }
}

const string JsonParser::formatDouble(const double value) {
    char buf[24];
    int i = sprintf(buf, "%.15f", value);
    //Remove trailing zeroes.
    while (i > -1 && (buf[i] == '\0' || buf[i] == '0')) {
        --i;
    }
    //If last char is a floating point/period add a single zero again
    if (i > -1 && buf[i] == '.') {
        ++i;
    }
    buf[i + 1] = '\0';
    return buf;
}

const string JsonParser::getPadding(const size_t depth) {
    string padding;
    for (size_t i = 0; i < depth; ++i) {
        padding += "    ";
    }
    return padding;
}