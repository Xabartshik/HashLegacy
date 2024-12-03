#pragma once
#include "HashLegacy.h"

// Шаблонный класс множества
template <typename T>
class Set {
private:
    // Хеш-таблица для хранения ключей
    HashTable<T> table;

public:
    // Конструктор множества
    Set(size_t capacity = 10, function<size_t(const T&)> hashFunction = [](const T& val) { return HashTable<T>::defaultHash(val); }, double maxLoadFactor = 0.7) : table(capacity, hashFunction, maxLoadFactor) {}

    // Добавление элемента в множество
    void insert(const T& value) {
        if (contains(value))
            return;
        table.insert(value);
    }

    // Удаление элемента из множества
    void erase(const T& value) {
        table.erase(value);
    }

    // Проверка наличия элемента в множестве
    bool contains(const T& value) const {
        return table.contains(value);
    }


    // Итератор для множества (используем итератор HashTable)
    typename HashTable<T>::iterator begin() {
        return table.begin();
    }

    typename HashTable<T>::iterator end() {
        return table.end();
    }

    // Размер множества
    size_t size() const {
        //  В HashTable не был реализован метод size(), поэтому  
        //  необходимо добавить его в класс HashTable
        size_t count = 0;
        for (auto it = begin(); it != end(); ++it) {
            count++;
        }
        return count;

        //  После добавления метода size() в HashTable, можно использовать:
        // return table.size();
    }

    bool empty() const {
        return size() == 0;
    }
};