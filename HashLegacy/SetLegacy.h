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


    // Итератор для множества (используем итератор HashTable). Указывает на начало множества
    typename HashTable<T>::iterator begin() {
        return table.begin();
    }
    // Итератор для множества (используем итератор HashTable). Указывает на начало множества
    typename HashTable<T>::iterator end() {
        return table.end();
    }

    // Размер множества
    size_t size() const {
        return table.size();
    }
    //Пустое ли множество
    bool empty() const {
        return size() == 0;
    }

    static void testAllMethods() {
        // Test insert, contains, and size
        Set<int> s1;
        s1.insert(1);
        s1.insert(2);
        s1.insert(3);
        assert(s1.size() == 3);
        assert(s1.contains(1));
        assert(s1.contains(2));
        assert(s1.contains(3));
        s1.insert(1); // повторная вставка
        assert(s1.size() == 3);


        // Test erase
        Set<int> s2;
        s2.insert(1);
        s2.insert(2);
        s2.insert(3);
        s2.erase(2);
        assert(s2.size() == 2);
        assert(s2.contains(1));
        assert(!s2.contains(2));
        assert(s2.contains(3));
        s2.erase(4); // удаление несуществующего
        assert(s2.size() == 2);


        // Test empty
        Set<int> s3;
        assert(s3.empty());
        s3.insert(1);
        assert(!s3.empty());


        // Test begin() and end()
        Set<int> s4;
        s4.insert(1);
        s4.insert(2);
        s4.insert(3);
        int count = 0;
        for (auto it = s4.begin(); it != s4.end(); ++it) {
            assert(s4.contains(*it));
            count++;
        }
        assert(count == s4.size());


        // Test custom hash and load factor
        auto hash = [](const std::string& str) { return str.length(); };
        Set<std::string> s5(5, hash, 0.9);
        s5.insert("a");
        s5.insert("bb");
        s5.insert("ccc");
        assert(s5.contains("a"));
        assert(s5.contains("bb"));
        assert(s5.contains("ccc"));
        assert(s5.size() == 3);

        std::cout << "All tests passed!" << std::endl;
    }
};