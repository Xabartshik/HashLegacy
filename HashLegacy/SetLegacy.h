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

    // Итератор для множества (используем итератор HashTable). Указывает на начало множества
    typename HashTable<T>::iterator begin() const {
        return table.begin();
    }
    // Итератор для множества (используем итератор HashTable). Указывает на начало множества
    typename HashTable<T>::iterator end() const {
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

    // Пересечение множеств
    Set<T> intersect(const Set<T>& other) const {
        Set<T> result;
        for (const T& value : *this) {
            if (other.contains(value)) {
                result.insert(value);
            }
        }
        return result;
    }

    // Объединение множеств
    Set<T> union_set(const Set<T>& other) const {
        Set<T> result = *this; // Копируем текущее множество
        for (const T& value : other) {
            result.insert(value);
        }
        return result;
    }

    // Разность множеств
    Set<T> difference(const Set<T>& other) const {
        Set<T> result;
        for (const T& value : *this) {
            if (!other.contains(value)) {
                result.insert(value);
            }
        }
        return result;
    }

    // Пересечение множеств (перегрузка оператора &)
    Set<T> operator&(const Set<T>& other) const {
        return intersect(other);
    }

    // Объединение множеств (перегрузка оператора |)
    Set<T> operator|(const Set<T>& other) const {
        return union_set(other);
    }

    // Разность множеств (перегрузка оператора -)
    Set<T> operator-(const Set<T>& other) const {
        return difference(other);
    }

    // Подмножество (перегрузка оператора <=)
    bool operator<=(const Set<T>& other) const {
        for (const T& value : *this) {
            if (!other.contains(value)) {
                return false;
            }
        }
        return true;
    }

    // Надмножество (перегрузка оператора >=)
    bool operator>=(const Set<T>& other) const {
        return other <= *this;
    }

    // Равенство множеств (перегрузка оператора ==)
    bool operator==(const Set<T>& other) const {
        return (*this <= other) && (other <= *this);
    }

    // Неравенство множеств (перегрузка оператора !=)
    bool operator!=(const Set<T>& other) const {
        return !(*this == other);
    }

    // Добавление элемента (перегрузка оператора +=)
    Set<T>& operator+=(const T& value) {
        insert(value);
        return *this;
    }

    // Добавление множества (перегрузка оператора += для множества)
    Set<T>& operator+=(const Set<T>& other) {
        for (const T& value : other) {
            insert(value);
        }
        return *this;
    }

    static void test_set_operations() {
        Set<int> s1;
        s1.insert(1);
        s1.insert(2);
        s1.insert(3);

        assert(s1.contains(1));
        assert(s1.contains(2));
        assert(s1.contains(3));
        assert(!s1.contains(4));
        assert(s1.size() == 3);

        s1.erase(2);
        assert(!s1.contains(2));
        assert(s1.size() == 2);

        Set<int> s2;
        s2.insert(3);
        s2.insert(4);
        s2.insert(5);

        Set<int> intersection = s1 & s2;
        assert(intersection.size() == 1);
        assert(intersection.contains(3));

        Set<int> union_set = s1 | s2;
        assert(union_set.size() == 4);
        assert(union_set.contains(1));
        assert(union_set.contains(3));
        assert(union_set.contains(4));
        assert(union_set.contains(5));

        Set<int> difference = s1 - s2;
        assert(difference.size() == 1);
        assert(difference.contains(1));

        assert(s1 != s2);

        s1 += 4;
        assert(s1.contains(4));


        Set<int> s3;
        s3.insert(1);
        s3.insert(3);
        s3.insert(4);
        assert(s1 == s3);
        assert(s1 >= s3);
        assert(s1 <= s3);

        s3.insert(5);
        assert(!(s1 >= s3));
        assert(s1 <= s3);
        assert(s1 != s3);


        s1 += s2;

        assert(s1.contains(5));


        Set<std::string> string_set;
        string_set.insert("hello");
        string_set.insert("world");

        assert(string_set.contains("hello"));
        assert(string_set.size() == 2);

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

        test_set_operations();




        std::cout << "All tests passed!" << std::endl;
    }
};