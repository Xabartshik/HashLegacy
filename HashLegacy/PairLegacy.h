#pragma once

// Класс пары ключ-значение
template <typename K, typename V>
class KeyValuePair {
public:
    K key;
    V value;
    
    KeyValuePair() : key(), value() {} // Конструктор по умолчанию

    KeyValuePair(const K& key, const V& value) : key(key), value(value) {}


    KeyValuePair(const KeyValuePair& other) : key(other.key), value(other.value) {} // Копирующий конструктор

    KeyValuePair& operator=(const KeyValuePair& other) { // Оператор присваивания
        if (this != &other) {
            key = other.key;
            value = other.value;
        }
        return *this;
    }

    bool operator==(const KeyValuePair& other) const { // Оператор сравнения
        return key == other.key;
    }

    bool operator!=(const KeyValuePair& other) const { // Оператор сравнения
        return !(*this == other);
    }
    // Дружественная хеш-функция для пары ключ-значение
    friend size_t hash_value(const KeyValuePair<K, V>& pair);
};
