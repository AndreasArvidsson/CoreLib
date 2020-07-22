#include "JsonNode.h"
#include <algorithm> //remove

using std::make_shared;

JsonNode::JsonNode() {
    _type = JsonNodeType::MISSING;
}

JsonNode::JsonNode(const JsonNodeType type) {
    _type = type;
}

JsonNode::JsonNode(const string& value) {
    _type = JsonNodeType::TEXT;
    _value.textValue = value;
}

JsonNode::JsonNode(const char* value) {
    _type = JsonNodeType::TEXT;
    _value.textValue = value;
}

JsonNode::JsonNode(const double value) {
    _type = JsonNodeType::NUMBER;
    _value.numberValue = value;
    _value.intValue = (long)value;
}

JsonNode::JsonNode(const long value) {
    _type = JsonNodeType::INTEGER;
    _value.numberValue = (double)value;
    _value.intValue = value;
}

JsonNode::JsonNode(const int value) {
    _type = JsonNodeType::INTEGER;
    _value.numberValue = (double)value;
    _value.intValue = value;
}

JsonNode::JsonNode(const bool value) {
    _type = JsonNodeType::BOOL;
    _value.boolValue = value;
}

JsonNode::~JsonNode() {
    _fields.clear();
    _items.clear();
}

const JsonNodeType JsonNode::getNodeType() const {
    return _type;
}

const bool JsonNode::isObject() const {
    return _type == JsonNodeType::OBJECT;
}

const bool JsonNode::isArray() const {
    return _type == JsonNodeType::ARRAY;
}

const bool JsonNode::isNumber() const {
    return _type == JsonNodeType::NUMBER || isInteger();
}

const bool JsonNode::isInteger() const {
    return _type == JsonNodeType::INTEGER;
}

const bool JsonNode::isBoolean() const {
    return _type == JsonNodeType::BOOL;
}

const bool JsonNode::isText() const {
    return _type == JsonNodeType::TEXT;
}

const bool JsonNode::isNull() const {
    return _type == JsonNodeType::NULL_;
}

const bool JsonNode::isMissingNode() const {
    return _type == JsonNodeType::MISSING;
}

const bool JsonNode::boolValue() const {
    return _value.boolValue;
}

const double JsonNode::doubleValue() const {
    return _value.numberValue;
}

const float JsonNode::floatValue() const {
    return (float)_value.numberValue;
}

const int JsonNode::intValue() const {
    return (int)_value.intValue;
}
const long JsonNode::longValue() const {
    return _value.intValue;
}

const string JsonNode::textValue() const {
    return _value.textValue;
}

const size_t JsonNode::size() const {
    return isObject() ? _fields.size() : _items.size();
}

const bool JsonNode::has(const size_t index) const {
    return index < _items.size();
}

const bool JsonNode::has(const string& fieldName) const {
    return _fields.find(fieldName) != _fields.end();
}

shared_ptr<JsonNode> JsonNode::get(const size_t index) const {
    if (!isArray()) {
        throw Error("JsonNode.get(int): Not an array");
    }
    return _items[index];
}

shared_ptr<JsonNode> JsonNode::get(const string& fieldName) const {
    if (!isObject()) {
        throw Error("JsonNode.get(string): Not an object");
    }
    return _fields.at(fieldName);
}

shared_ptr<JsonNode> JsonNode::path(const size_t index) const {
    if (isArray() && has(index)) {
        return _items[index];
    }
    return make_shared<JsonNode>();
}

shared_ptr<JsonNode> JsonNode::path(const string& fieldName) const {
    if (isObject()) {
        const auto& got = _fields.find(fieldName);
        if (got != _fields.end()) {
            return got->second;
        }
    }
    return make_shared<JsonNode>();
}

const vector<string> JsonNode::getOrder() const {
    return _fieldOrder;
}

const unordered_map<string, shared_ptr<JsonNode>> JsonNode::getFields() const {
    return _fields;
}

void JsonNode::add(shared_ptr<JsonNode> pJsonNode) {
    if (!isArray()) {
        throw Error("JsonNode.add(): Not an array");
    }
    _items.push_back(pJsonNode);
}

void JsonNode::renameField(const string& oldField, const string& newField) {
    if (!isObject()) {
        throw Error("JsonNode.renameField(): Not an object");
    }
    if (!has(oldField)) {
        throw Error("JsonNode.renameField(): Can't rename unexisting field");
    }
    _fields[newField] = _fields[oldField];
    _fields.erase(oldField);
    const auto it = find(_fieldOrder.begin(), _fieldOrder.end(), oldField);
    *it = newField;
}

void JsonNode::put(const string& fieldName, shared_ptr<JsonNode> pJsonNode) {
    if (!isObject()) {
        throw Error("JsonNode.put(): Not an object");
    }
    if (!has(fieldName)) {
        _fieldOrder.push_back(fieldName);
    }
    _fields[fieldName] = pJsonNode;
}

void JsonNode::remove(const size_t index) {
    if (!isArray()) {
        throw Error("JsonNode.remove(int): Not an array");
    }
    _items.erase(_items.begin() + index);
}

void JsonNode::remove(const string& fieldName) {
    if (!isObject()) {
        throw Error("JsonNode.remove(string): Not an object");
    }
    if (_fields.erase(fieldName)) {
        _fieldOrder.erase(std::remove(_fieldOrder.begin(), _fieldOrder.end(), fieldName), _fieldOrder.end());
    }
}

void JsonNode::clear() {
    if (!isObject() && !isArray()) {
        throw Error("JsonNode.clear(): Not an object or array");
    }
    _fields.clear();
    _fieldOrder.clear();
    _items.clear();
}