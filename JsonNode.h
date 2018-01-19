#pragma once
#include <string>
#include <exception>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Error.h"

//class JsonNode;
//typedef typename std::unordered_map<std::string, JsonNode>::const_iterator const_iterator;

enum class JsonNodeType {
	OBJECT,
	ARRAY,
	NUMBER,
	INTEGER,
	TEXT,
	BOOL,
	NULL_,
	MISSING
};

class Value {
public:
	double numberValue;
	long intValue;
	bool boolValue;
	std::string textValue;

	Value() {
		numberValue = 0;
		intValue = 0;
		boolValue = false;
	}

};

class JsonNode {
public:

	static void destroyStatic();

	JsonNode(JsonNode*) = delete;
	JsonNode(const JsonNode*) = delete;
	JsonNode(const JsonNode&) = delete;

	JsonNode();
	JsonNode(const JsonNodeType type);
	JsonNode(const std::string &value);
	JsonNode(const double value);
	JsonNode(const int value);
	JsonNode(const bool value);
	~JsonNode();

	const JsonNodeType getNodeType() const;
	const bool isObject() const;
	const bool isArray() const;
	const bool isNumber() const;
	const bool isInteger() const;
	const bool isBoolean() const;
	const bool isText() const;
	const bool isNull() const;
	const bool isMissingNode() const;
	
	const bool boolValue() const;
	const double doubleValue() const;
	const float floatValue() const;
	const int intValue() const;
	const long longValue() const;
	const std::string textValue() const;

	const size_t size() const;
	const bool has(const size_t index) const;
	const bool has(const std::string &fieldName) const;
	JsonNode* get(const size_t index) const;
	JsonNode* get(const std::string &fieldName) const;
	JsonNode* path(const size_t index) const;
	JsonNode* path(const std::string &fieldName) const;
	void add(JsonNode *pJsonNode);
	void put(const std::string fieldName, JsonNode *pJsonNode);
	void put(const std::string fieldName, const std::string &text);
	
	const std::vector<std::string> getOrder() const;


	/*virtual const_iterator begin() const {
		return nullptr;
	}*/

	//virtual const_iterator begin() const {
	//	throw std::exception("Not an object");
	//}

	//virtual const_iterator end() const {
	//	throw std::exception("Not an object");
	//}

	/*const_iterator begin() const {
		return _fields.begin();
	}

	const_iterator end() const {
		return _fields.end();
	}*/

private:
	static JsonNode *_pMissingNode;

	static JsonNode* getMissingNode();

	JsonNodeType _type;
	Value _value;
	std::unordered_map<std::string, JsonNode*> _fields;
	std::vector<std::string> _fieldOrder;
	std::vector<JsonNode*> _items;

};

