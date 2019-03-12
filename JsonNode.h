#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Error.h"

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
	JsonNode(const char *value);
	JsonNode(const double value);
	JsonNode(const long value);
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
	const std::vector<std::string> getOrder() const;
    void renameField(const std::string &oldField, const std::string &newField);
	const std::unordered_map<std::string, JsonNode*> getFields() const;
	void add(JsonNode *pJsonNode);
	void put(const std::string &fieldName, JsonNode *pJsonNode);

	template<typename T>
	void add(const T &value) {
		if (!isArray()) {
			throw Error("Not an array");
		}
		_items.push_back(new JsonNode(value));
	}

	template<typename T>
	void put(const std::string &fieldName, const T &value) {
		if (!isObject()) {
			throw Error("Not an object");
		}
		if (has(fieldName)) {
			//Have to delete existing or get memory leaks.
			delete _fields[fieldName];
		}
		//New field name/key. Add to list.
		else {
			_fieldOrder.push_back(fieldName);
		}
		_fields[fieldName] = new JsonNode(value);
	}

	void remove(const size_t index);
	void remove(const std::string &fieldName);
	void clear();

private:
	static JsonNode *_pMissingNode;

	static JsonNode* getMissingNode();

	JsonNodeType _type;
	Value _value;
	std::unordered_map<std::string, JsonNode*> _fields;
	std::vector<std::string> _fieldOrder;
	std::vector<JsonNode*> _items;

};

