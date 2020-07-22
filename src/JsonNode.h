#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Error.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::unordered_map;

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
    string textValue;

    Value() {
        numberValue = 0;
        intValue = 0;
        boolValue = false;
    }

};

class JsonNode {
public:
    JsonNode(JsonNode*) = delete;
    JsonNode(const JsonNode*) = delete;
    JsonNode(const JsonNode&) = delete;

    JsonNode();
    JsonNode(const JsonNodeType type);
    JsonNode(const string& value);
    JsonNode(const char* value);
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
    const string textValue() const;

    const size_t size() const;
    const bool has(const size_t index) const;
    const bool has(const string& fieldName) const;
    shared_ptr<JsonNode> get(const size_t index) const;
    shared_ptr<JsonNode> get(const string& fieldName) const;
    shared_ptr<JsonNode> path(const size_t index) const;
    shared_ptr<JsonNode> path(const string& fieldName) const;
    const vector<string> getOrder() const;
    void renameField(const string& oldField, const string& newField);
    const unordered_map<string, shared_ptr<JsonNode>> getFields() const;
    void add(shared_ptr<JsonNode> pJsonNode);
    void put(const string& fieldName, shared_ptr<JsonNode> pJsonNode);

    template<typename T>
    void add(const T& value) {
        if (!isArray()) {
            throw Error("Not an array");
        }
        _items.push_back(make_shared<JsonNode>(value));
    }

    template<typename T>
    void put(const string& fieldName, const T& value) {
        if (!isObject()) {
            throw Error("Not an object");
        }
        //New field name/key. Add to list.
        else {
            _fieldOrder.push_back(fieldName);
        }
        _fields[fieldName] = make_shared<JsonNode>(value);
    }

    void remove(const size_t index);
    void remove(const string& fieldName);
    void clear();

private:
    JsonNodeType _type;
    Value _value;
    unordered_map<string, shared_ptr<JsonNode>> _fields;
    vector<string> _fieldOrder;
    vector<shared_ptr<JsonNode>> _items;

};