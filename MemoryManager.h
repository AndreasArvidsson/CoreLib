#pragma once

#ifdef DEBUG_MEMORY

#include "Log.h"

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

    const bool get(const K &key, V &value) {
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i].key == key) {
                value = _data[i].value;
                return true;
            }
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

    void print() const {
        for (size_t i = 0; i < _size; ++i) {
            printf("%p\n", _data[i].key);
        }
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
        return _map.size() + _mapArr.size() > 0;
    }

    void displayInfo() {
        if (_map.size() + _mapArr.size() > 0) {
            LOG_ERROR("MEMORY LEAK DETECTED! p: %lu, p[]: %lu", _map.size(), _mapArr.size());
        }
        LOG_INFO("Max memory usage: %.1fkB", _maxMemoryUsage / 1000.0);
        LOG_INFO("Max num pointers used: %lu", _maxPointers);
    }

    void assertNoLeak() const {
        if (getInstance()->hasLeak()) {
            getInstance()->displayInfo();
            system("PAUSE");
            exit(EXIT_FAILURE);
        }
    }

    void* mallocMemory(const size_t size, const bool isArr) {
        //Add 4 * padding to the size.
        void *p = malloc(size + _paddingSize * 4);
        size_t address = reinterpret_cast<size_t> (p);
        //if (address == 0x0000027964C740F0) {
        //	int a = 2;
        //}
        if (isArr) {
            add(_mapArr, p, size);
        }
        else {
            add(_map, p, size);
        }
        size_t numPointers = _mapArr.size() + _map.size();
        _maxPointers = _maxPointers > numPointers ? _maxPointers : numPointers;
        return p;
    }

    void freeMemory(void*const p, const bool isArr) {
        if (isArr) {
            remove(_mapArr, p);
        }
        else {
            remove(_map, p);
        }
        free(p);
    }

private:
    static MemoryManager* _pInstance;

    size_t _padding, _paddingSize, _memoryUsage, _maxMemoryUsage, _maxPointers;
    Map<void*, size_t> _map, _mapArr;

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
        _mapArr.init(500);
    }

    void add(Map<void*, size_t> &map, void* p, size_t size) {
        if (map.contains(p)) {
            LOG_ERROR("***** POINTER ALREADY IN USE: 0x%p *****", p);
        }
        _memoryUsage += size;
        if (_memoryUsage > _maxMemoryUsage) {
            _maxMemoryUsage = _memoryUsage;
        }
        //Add padding at end data.
        addPadding(p, size);
        //Store pointer in map.
        map.put(p, size);
    }

    void remove(Map<void*, size_t> &map, void*const p) {
        //Get size from map.
        size_t size;
        if (!map.get(p, size)) {
            //LOG_ERROR("*** CAN'T FIND POINTER: 0x%p ***", p);
            return;
        }
        _memoryUsage -= size;
        //Check if padding is valid.
        checkPadding(p, size);
        //Remove pointer from map.
        map.remove(p);
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

#endif