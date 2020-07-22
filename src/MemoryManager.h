#pragma once
#define NOMINMAX
#include <algorithm> //max

using std::max;

#ifndef LOG_INFO
#include "Log.h"
#endif

/*************************************************
********** MAP CLASS
*************************************************/

template <typename K, typename V>
class KeyValuePair {
public:
	K key;
	V value;

	void set(K k, V v) {
		key = k;
		value = v;
	}
};

template <typename K, typename V>
class Map {
public:

	Map() {
		init(1000);
	}

	Map(const size_t capacity) {
		init(capacity);
	}

	void init(const size_t capacity) {
		_capacity = capacity;
		_data = (KeyValuePair<K, V>*) malloc(sizeof(KeyValuePair<K, V>) * capacity);
		_size = 0;
	}

	virtual ~Map() {
		free(_data);
	}

	const bool put(const K &key, const V &value) {
		for (size_t i = 0; i < _size; ++i) {
			if (_data[i].key == key) {
				_data[i].value = value;
				return true;
			}
		}
		return add(key, value);
	}

	const bool get(const K &key, V *pResult) const {
		for (size_t i = 0; i < _size; ++i) {
			if (_data[i].key == key) {
				memcpy(pResult, &_data[i].value, sizeof(V));
				//value = _data[i].value;
				return true;
			}
		}
		return false;
	}

	const bool getByIndex(const size_t index, V *pResult) const {
		if (index < _size) {
			memcpy(pResult, &_data[index].value, sizeof(V));
			//value = _data[index];
			return true;
		}
		return false;
	}

	const bool contains(const K &key) const {
		for (size_t i = 0; i < _size; ++i) {
			if (_data[i].key == key) {
				return true;
			}
		}
		return false;
	}

	const bool remove(const K &key) {
		for (size_t i = 0; i < _size; ++i) {
			if (_data[i].key == key) {
				_data[i].key = _data[_size - 1].key;
				_data[i].value = _data[_size - 1].value;
				--_size;
				return true;
			}
		}
		return false;
	}

	const size_t size() const {
		return _size;
	}

	const size_t capacity() const {
		return _capacity;
	}

	KeyValuePair<K, V> getData() {
		return _data;
	}

private:
	KeyValuePair<K, V> *_data;
	size_t _capacity, _size;

	const bool add(K key, V value) {
		if (_size == _capacity) {
			return false;
		}
		_data[_size++].set(key, value);
		return true;
	}

	Map(const Map& orig);

};

/*************************************************
********** POINTER CLASS
*************************************************/

class Pointer {
public:
	void *address;
	size_t size;
	bool isArray;
	Pointer() {}
	Pointer(void *address, size_t size, bool isArray) : address(address), size(size), isArray(isArray) {}
	Pointer(const Pointer &ref) : address(ref.address), size(ref.size), isArray(ref.isArray) {}
};

/*************************************************
********** MEMORY MANAGER CLASS
*************************************************/

class MemoryManager {
public:

	static MemoryManager* getInstance() {
		if (!_pInstance) {
			_pInstance = (MemoryManager*)malloc(sizeof(MemoryManager));
			_pInstance->init();
		}
		return _pInstance;
	}

	const bool hasLeak() const {
		return _map.size() > 0;
	}

	void displayInfo() const {
		if (_map.size() > 0) {
			size_t numP = 0;
			size_t numArrP = 0;
			Pointer pointer;
			for (size_t i = 0; i < _map.size(); ++i) {
				_map.getByIndex(i, &pointer);
				if (pointer.isArray) {
					++numArrP;
				}
				else {
					++numP;
				}
			}
			LOG_ERROR("MEMORY LEAK DETECTED! p: %zu, p[]: %zu", numP, numArrP);
		}
		LOG_INFO("Max memory usage: %.1fkB", _maxMemoryUsage / 1000.0);
		LOG_INFO("Max num pointers used: %zu", _maxPointers);
	}

	void displayPointers() const {
		Pointer pointer;
		LOG_INFO("Address\t\t\tSize\tisArray");
		for (size_t i = 0; i < _map.size(); ++i) {
			_map.getByIndex(i, &pointer);
			LOG_INFO("%p\t%zu\t%d", pointer.address, pointer.size, pointer.isArray);
		}
	}

	void assertNoLeak() const {
		if (getInstance()->hasLeak()) {
			getInstance()->displayInfo();
			LOG_NL();
			getInstance()->displayPointers();
			LOG_NL();
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}

	void* mallocMemory(const size_t size, const bool isArray) {
		//Add 4 * padding to the size.
		void *p = malloc(size + _paddingSize * 4);

		if (_map.contains(p)) {
			LOG_ERROR("***** POINTER ALREADY IN USE: 0x%p *****", p);
		}

		//Add padding at end data.
		addPadding(p, size);
		//Store pointer in map.
		_map.put(p, Pointer(p, size, isArray));

		//Update stats
		_memoryUsage += size;
		_maxMemoryUsage = max(_maxMemoryUsage, _memoryUsage);
		_maxPointers = max(_maxPointers, _map.size());
		return p;
	}

	void freeMemory(void*const p, const bool isArr) {
		if (p == nullptr) {
			return;
		}
		//Get size from map.
		Pointer pointer;
		if (!_map.get(p, &pointer)) {
			//LOG_ERROR("*** CAN'T FIND POINTER: 0x%p ***", p);
			return;
		}
		if (pointer.isArray && !isArr) {
			LOG_ERROR("*** DELETE MISSMATCH(new[], delete): 0x%p ***", p);
			return;
		}
		if (!pointer.isArray && isArr) {
			LOG_ERROR("*** DELETE MISSMATCH(new, delete[]): 0x%p ***", p);
			return;
		}
		_memoryUsage -= pointer.size;
		//Check if padding is valid.
		checkPadding(p, pointer.size);
		//Remove pointer from map.
		_map.remove(p);
		//Actually free memory.
		free(p);
	}

private:
	static MemoryManager* _pInstance;

	size_t _padding, _paddingSize, _memoryUsage, _maxMemoryUsage, _maxPointers;
	Map<void*, Pointer> _map;

	void addPadding(void *const p, const size_t size) {
		//Copy padding data to the memory at the end.
		memcpy((char*)p + size, &_padding, _paddingSize);
		memcpy((char*)p + size + _paddingSize, &_padding, _paddingSize);
		memcpy((char*)p + size + _paddingSize * 2, &_padding, _paddingSize);
		memcpy((char*)p + size + _paddingSize * 3, &_padding, _paddingSize);
	}

	void checkPadding(void *const p, const size_t size) {
		size_t *padding = (size_t*)((char*)p + size);
		if (padding[0] != _padding || padding[1] != _padding || padding[2] != _padding || padding[3] != _padding) {
			LOG_ERROR("*** HEAP CORRUPTION DETECTED! ***");
		}
	}

	void init() {
		_padding = 0xAAAAAAAA;
		_paddingSize = sizeof(_padding);
		_memoryUsage = _maxMemoryUsage = _maxPointers = 0;
		_map.init(500);
		//_mapArr.init(500);
	}

};

MemoryManager*  MemoryManager::_pInstance = nullptr;

void* operator new(const size_t size) {
	if (!size) {
		LOG_ERROR("NEW SIZE IS ZERO");
	}
	return MemoryManager::getInstance()->mallocMemory(size, false);
}

void* operator new [](const size_t size) {
	if (!size) {
		LOG_ERROR("NEW[] SIZE IS ZERO");
	}
	return MemoryManager::getInstance()->mallocMemory(size, true);
}

void operator delete(void*const p) {
	MemoryManager::getInstance()->freeMemory(p, false);
}

void operator delete [](void*const p) {
	MemoryManager::getInstance()->freeMemory(p, true);
}