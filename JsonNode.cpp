#include "JsonNode.h"
#include <algorithm> //std::remove

JsonNode *JsonNode::_pMissingNode = nullptr;

JsonNode* JsonNode::getMissingNode() {
	if (!_pMissingNode) {
		_pMissingNode = new JsonNode();
	}
	return _pMissingNode;
}

void JsonNode::destroyStatic() {
	delete _pMissingNode;
	_pMissingNode = nullptr;
}

JsonNode::JsonNode() {
	_type = JsonNodeType::MISSING;
}

JsonNode::JsonNode(const JsonNodeType type) {
	_type = type;
}

JsonNode::JsonNode(const std::string &value) {
	_type = JsonNodeType::TEXT;
	_value.textValue = value;
}

JsonNode::JsonNode(const char *value) {
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
	for (auto &e : _fields) {
		delete e.second;
	}
    _fields.clear();
	for (JsonNode *p : _items) {
		delete p;
	}
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

const std::string JsonNode::textValue() const {
	return _value.textValue;
}

const size_t JsonNode::size() const {
	return isObject() ? _fields.size() : _items.size();
}

const bool JsonNode::has(const size_t index) const {
	return index < _items.size();
}

const bool JsonNode::has(const std::string &fieldName) const {
	return _fields.find(fieldName) != _fields.end();
}

JsonNode* JsonNode::get(const size_t index) const {
	if (!isArray()) {
		throw Error("Not an array");
	}
	return _items[index];
}

JsonNode* JsonNode::get(const std::string &fieldName) const {
	if (!isObject()) {
		throw Error("Not an object");
	}
	return _fields.at(fieldName);
}

JsonNode* JsonNode::path(const size_t index) const {
	if (isArray() && has(index)) {
		return _items[index];
	}
	return getMissingNode();
}

JsonNode* JsonNode::path(const std::string &fieldName) const {
	if (isObject()) {
		const auto &got = _fields.find(fieldName);
		if (got != _fields.end()) {
			return got->second;
		}
	}
	return getMissingNode();
}

const std::vector<std::string> JsonNode::getOrder() const {
	return _fieldOrder;
}

const std::unordered_map<std::string, JsonNode*> JsonNode::getFields() const {
	return _fields;
}

void JsonNode::add(JsonNode *pJsonNode) {
	if (!isArray()) {
		throw Error("Not an array");
	}
	_items.push_back(pJsonNode);
}

void JsonNode::renameField(const std::string &oldField, const std::string &newField) {
    if (!isObject()) {
        throw Error("Not an object");
    }
    if (!has(oldField)) {
        throw Error("Cant rename unexisting field");
    }
    _fields[newField] = _fields[oldField];
    _fields.erase(oldField);
    const auto it = std::find(_fieldOrder.begin(), _fieldOrder.end(), oldField);
    *it = newField;
}

void JsonNode::put(const std::string &fieldName, JsonNode *pJsonNode) {
	if (!isObject()) {
		throw Error("Not an object");
	}
	if (!has(fieldName)) {
		_fieldOrder.push_back(fieldName);
	}
	_fields[fieldName] = pJsonNode;
}

void JsonNode::remove(const size_t index) {
	if (!isArray()) {
		throw Error("Not an array");
	}
	_items.erase(_items.begin() + index);
}

void JsonNode::remove(const std::string &fieldName) {
	if (!isObject()) {
		throw Error("Not an object");
	}
	if (_fields.erase(fieldName)) {
		_fieldOrder.erase(std::remove(_fieldOrder.begin(), _fieldOrder.end(), fieldName), _fieldOrder.end());
	}
}

void JsonNode::clear() {
	if (!isObject() && !isArray()) {
		throw Error("Not an object or array");
	}
	_fields.clear();
	_fieldOrder.clear();
	_items.clear();
}