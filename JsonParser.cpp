#include "JsonParser.h"
#include <cctype> //std::isdigit
#include "File.h"
#include "Error.h"

JsonNode* JsonParser::fromString(const std::string &str) {
	const char *p = str.c_str();
	size_t row = 1;
	removeBlank(p, row);
	return parseObject(p, row);
}

JsonNode* JsonParser::fromFile(const File &file) {
	std::string content;
	if (!file.getData(content)) {
		throw Error("JSON - Can't read file '%s'", file.getPath().c_str());
	}
	return fromString(content);
}

void JsonParser::toFile(const File &file, const JsonNode &node) {
	if (!file.setData(JsonParser::toString(node))) {
		throw Error("JSON - Cant write content to file '%s'", file.getPath().c_str());
	}
}

JsonNode* JsonParser::parseObject(const char *&p, size_t &row) {
	JsonNode *pObjectNode = new JsonNode(JsonNodeType::OBJECT);
	try {
		match(p, '{', row);
		removeBlank(p, row);
		while (*p != '}' && *p != '\0') {
			parseField(p, pObjectNode, row);
		}
		match(p, '}', row);
	}
	catch (const Error &e) {
		delete pObjectNode;
		throw e;
	}
	return pObjectNode;
}

void JsonParser::parseField(const char *&p, JsonNode *pObjectNode, size_t &row) {
	std::string name = matchFieldID(p, row);
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

JsonNode* JsonParser::parseArray(const char *&p, size_t &row) {
	JsonNode *pArrayNode = new JsonNode(JsonNodeType::ARRAY);
	try {
		match(p, '[', row);
		removeBlank(p, row);
		while (*p != ']' && *p != '\0') {
			parseItem(p, pArrayNode, row);
		}
		match(p, ']', row);
	}
	catch (const Error &e) {
		delete pArrayNode;
		throw e;
	}
	return pArrayNode;
}

void JsonParser::parseItem(const char *&p, JsonNode *pArrayNode, size_t &row) {
	pArrayNode->add(matchValue(p, row));
	removeBlank(p, row);
	if (*p == ',') {
		match(p, ',', row);
		removeBlank(p, row);
		parseItem(p, pArrayNode, row);
	}
}

std::string JsonParser::matchFieldID(const char *&p, size_t &row) {
	match(p, '"', row);
	std::string name = next(p, '"', row);
	if (name.size() == 0) {
		throw Error("JSON(%lu) - Expected ID near '%s'", row, next(p, '\n', row));
	}
	match(p, '"', row);
	return name;
}

JsonNode* JsonParser::matchValue(const char *&p, size_t &row) {
	if (*p == '{') {
		return parseObject(p, row);
	}
	if (*p == '[') {
		return parseArray(p, row);
	}
	if (*p == '"') {
		match(p, '"', row);
		std::string value = next(p, '"', row);
		JsonNode *pNode = new JsonNode(value);
		match(p, '"', row);
		return pNode;
	}
	std::string tmp = next(p).c_str();
	if (strcmp(tmp.c_str(), "true") == 0) {
		return new JsonNode(true);
	}
	else if (strcmp(tmp.c_str(), "false") == 0) {
		return new JsonNode(false);
	}
	else if (isNumber(tmp)) {
		if (isInteger(tmp)) {
			return new JsonNode(atoi(tmp.c_str()));
		}
		else {
			return new JsonNode(atof(tmp.c_str()));
		}
	}
	throw Error("JSON(%lu) - Unknown value '%s' near '%s'", row, tmp.c_str(), next(p, '\n', row).c_str());
}

void JsonParser::match(const char *&str, const char expected, size_t &row) {
	if (*str != expected) {
		throw Error("JSON(%lu) - Expected '%c', found '%s'", row, expected, next(str, '\n', row).c_str());
	}
	++str; //Step over the char.
}

void JsonParser::removeBlank(const char *&p, size_t &row) {
	while (*p == ' ' || *p == '\n' || *p == '\t') {
		if (*p == '\n') {
			++row;
		}
		++p;
	}
}

std::string JsonParser::next(const char *&p, const char breakChar, size_t &row) {
	const char *begin = p;
	while (*p != breakChar && *p != '\0') {
		if (*p == '\n') {
			++row;
		}
		++p;
	}
	return std::string(begin, p);
}

std::string JsonParser::next(const char *&p) {
	const char *begin = p;
	while (isalnum(*p) || *p == '.' || *p == '-') {
		++p;
	}
	return std::string(begin, p);
}

const bool JsonParser::isNumber(const std::string &s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || *it == '-' || *it == '.')) {
		++it;
	}
	return !s.empty() && it == s.end();
}

const bool JsonParser::isInteger(const std::string &s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || *it == '-')) {
		++it;
	}
	return !s.empty() && it == s.end();
}

const std::string JsonParser::toString(const JsonNode &node, const size_t depth) {
	std::string padding = getPadding(depth);
	switch (node.getNodeType()) {
	case JsonNodeType::NUMBER:
		return formatDouble(node.doubleValue());
	case JsonNodeType::INTEGER:
		return std::to_string(node.intValue());
	case JsonNodeType::TEXT:
		return "\"" + node.textValue() + "\"";
	case JsonNodeType::BOOL:
		return std::string(node.boolValue() ? "true" : "false");
	case JsonNodeType::NULL_:
		return std::string("null");
	case JsonNodeType::OBJECT: {
		std::string out = "{";
		bool addComma = false;
		for (const std::string &fieldName : node.getOrder()) {
			const JsonNode *pField = node.get(fieldName);
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
		std::string out = "[";
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
		return padding + std::string("ERROR!!!");
	}
}

const std::string JsonParser::formatDouble(const double value) {
	char buf[24];
	int i = sprintf(buf, "%.15f", value);
	//Remove trailing zeroes.
	while (i > -1 && (buf[i] == '\0' || buf[i] == '0')) {
		--i;
	}
	//If last char is a floating point/period add a single zero again
	if (buf[i] == '.') {
		++i;
	}
	buf[i + 1] = '\0';
	return buf;
}

const std::string JsonParser::getPadding(const size_t depth) {
	std::string padding;
	for (size_t i = 0; i < depth; ++i) {
		padding += "    ";
	}
	return padding;
}