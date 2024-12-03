#include "HashLegacy.h"
#include "PairLegacy.h"
//HashTable<Key>::
using namespace std;

// ... (Определения хеш-функций djb2Hash, defaultHash, xorHash, multiplyHash, 
// sumHash, productHash, xorShiftHash, quadraticHash остаются без изменений) ...

// Хеш-функция для пары, хэширующая только ключ
template <typename K, typename V>
size_t hash_value(const KeyValuePair<K, V>& pair) {
    return djb2Hash<K>(pair.key);
}

template <typename Key, typename Value>
class Dictionary {
private:
    // Хеш-таблица для хранения пар ключ-значение
    HashTable<KeyValuePair<Key, Value>> table;

public:
    // Конструктор словаря
    Dictionary(size_t capacity, function<size_t(const KeyValuePair<Key, Value>&)> hashFunction = [](const KeyValuePair<Key, Value>& p) { return HashTable<Key>::defaultHash(p.key); }, double maxLoadFactor = 0.7)
        : table(capacity, hashFunction, maxLoadFactor) {}

    // Вставка пары ключ-значение в словарь
    void insert(const Key& key, const Value& value) {
        table.insert(KeyValuePair<Key, Value>(key, value));
    }

    // Удаление пары ключ-значение из словаря
    void erase(const Key& key) {
        table.erase(KeyValuePair<Key, Value>(key, Value())); // V() создает дефолтное значение для V, необходимо для поиска и удаления
    }


    // Получение значения по ключу. 
    Value& operator[](const Key& key) {
        KeyValuePair<Key, Value> tempPair(key, Value()); // Создаем временную пару для поиска
        size_t index = table.hash(tempPair) % table.capacity();
        for (auto& pair : table.getBucket(index)) { // Итерируемся только по нужному бакету
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }

    // Получение значения по ключу (константная версия).
    const Value& operator[](const Key& key) const {
        KeyValuePair<Key, Value> tempPair(key, Value()); // Создаем временную пару для поиска
        size_t index = table.hash(tempPair) % table.capacity();
        for (const auto& pair : table.getBucket(index)) { // Итерируемся только по нужному бакету
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }



    // Проверка наличия ключа в словаре
    bool contains(const Key& key) const {
        // Создаем временную пару с фиктивным значением для поиска
        return table.contains(KeyValuePair<Key, Value>(key, Value()));
    }



    // Поиск значения по ключу.
    Value* find(const Key& key) {
        KeyValuePair<Key, Value> tempPair(key, Value()); // Создаем временную пару для поиска
        size_t index = table.hash(tempPair) % table.size();
        for (auto& pair : table.getBucket(index)) { // Итерируемся только по нужному бакету
            if (pair.key == key) {
                return &pair.value;
            }
        }
        return nullptr;
    }

    // Получение значения по ключу (константная версия).
    const Value* find(const Key& key) const {
        KeyValuePair<Key, Value> tempPair(key, Value()); // Создаем временную пару для поиска
        size_t index = table.hash(tempPair) % table.size();
        for (const auto& pair : table.getBucket(index)) { // Итерируемся только по нужному бакету
            if (pair.key == key) {
                return &pair.value;
            }
        }
        return nullptr;
    }


    typename HashTable<KeyValuePair<Key, Value>>::iterator begin() {
        return table.begin();
    }

    typename HashTable<KeyValuePair<Key, Value>>::iterator end() {
        return table.end();
    }
};