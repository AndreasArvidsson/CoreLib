#include "JsonParser.h"
#include <cctype> //std::isdigit
#include "File.h"
#include "Error.h"

JsonNode* JsonParser::fromString(const std::string &str) {
	const char *p = str.c_str();
	removeBlank(p);
	return parseObject(p);
}

JsonNode* JsonParser::fromFile(const File &file) {
	std::string content;
	if (!file.getData(content)) {
		throw Error("Cant read json file '%s'", file.getPath().c_str());
	}
	return fromString(content);
}

void JsonParser::toFile(const File &file, const JsonNode &node) {
	if (!file.setData(JsonParser::toString(node))) {
		throw Error("Cant write file content");
	}
}

JsonNode* JsonParser::parseObject(const char *&p) {
	JsonNode *pObjectNode = new JsonNode(JsonNodeType::OBJECT);
	try {
		match(p, '{');
		removeBlank(p);
		while (*p == '"') {
			std::string name = matchName(p);
			removeBlank(p);
			match(p, ':');
			removeBlank(p);
			pObjectNode->put(name, matchValue(p));
			removeBlank(p);
			if (*p == ',') {
				match(p, ',');
			}
			removeBlank(p);
		}
		match(p, '}');
	}
	catch (const Error &e) {
		delete pObjectNode;
		throw e;
	}
	return pObjectNode;
}

JsonNode* JsonParser::parseArray(const char *&p) {
	JsonNode *pArrayNode = new JsonNode(JsonNodeType::ARRAY);
	try {
	match(p, '[');
	removeBlank(p);
	while (*p != ']' && *p != '\0') {
		pArrayNode->add(matchValue(p));
		removeBlank(p);
		if (*p == ',') {
			match(p, ',');
			removeBlank(p);
		}
	}
	removeBlank(p);
	match(p, ']');
	}
	catch (const Error &e) {
		delete pArrayNode;
		throw e;
	}
	return pArrayNode;
}

std::string JsonParser::matchName(const char *&p) {
	match(p, '"');
	std::string name = next(p, '"');
	if (name.size() == 0) {
		throw Error("JSON parse failed. Expected name near: %s", p);
	}
	match(p, '"');
	return name;
}

JsonNode* JsonParser::matchValue(const char *&p) {
	if (*p == '{') {
		return parseObject(p);
	}
	if (*p == '[') {
		return parseArray(p);
	}
	if (*p == '"') {
		match(p, '"');
		std::string value = next(p, '"');
		JsonNode *pNode = new JsonNode(value);
		match(p, '"');
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
	throw Error("JSON parse failed. Unknown value: \"%s\"", tmp.c_str());
}

void JsonParser::match(const char *&str, const char expected) {
	if (*str != expected) {
		throw Error("JSON parse failed. Expected: \"%c\", found: \"%c\"", expected, *str);
	}
	++str; //Step over the char.
}

void JsonParser::removeBlank(const char *&p) {
	while (*p == ' ' || *p == '\n' || *p == '\t') {
		++p;
	}
}

std::string JsonParser::next(const char *&p, const char breakChar) {
	const char *begin = p;
	while (*p != breakChar) {
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