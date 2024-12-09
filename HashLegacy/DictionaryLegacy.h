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
        KeyValuePair<Key, Value> tempPair(key, Value());
        size_t index = table.hash(tempPair) % table.capacity();
        size_t originalIndex = index;

        do {
            // Проверяем, занята ли ячейка
            if (table.isOccupied(index)) {
                KeyValuePair<Key, Value>& pair = table.getListAtIndex(index);
                if (pair.key == key) {
                    return pair.value;
                }
            }
            index = (index + 1) % table.capacity();
        } while (index != originalIndex);

        throw runtime_error("Key not found");
    }

    // Получение значения по ключу (константная версия).
    const Value& operator[](const Key& key) const {
        KeyValuePair<Key, Value> tempPair(key, Value());
        size_t index = table.hash(tempPair) % table.capacity();
        size_t originalIndex = index;

        do {
            // Проверяем, занята ли ячейка
            if (table.isOccupied(index)) {
                const KeyValuePair<Key, Value>& pair = table.getListAtIndex(index);
                if (pair.key == key) {
                    return pair.value;
                }
            }
            index = (index + 1) % table.capacity();
        } while (index != originalIndex);

        throw runtime_error("Key not found");
    }

    // Поиск значения по ключу.
    Value* find(const Key& key) {
        KeyValuePair<Key, Value> tempPair(key, Value());
        size_t index = table.hash(tempPair) % table.capacity();
        size_t originalIndex = index;

        do {
            // Проверяем, занята ли ячейка
            if (table.isOccupied(index)) {
                KeyValuePair<Key, Value>& pair = table.getListAtIndex(index);
                if (pair.key == key) {
                    return &pair.value;
                }
            }
            index = (index + 1) % table.capacity();
        } while (index != originalIndex);

        return nullptr;
    }

    // Поиск значения по ключу (константная версия).
    const Value* find(const Key& key) const {
        KeyValuePair<Key, Value> tempPair(key, Value());
        size_t index = table.hash(tempPair) % table.capacity();
        size_t originalIndex = index;

        do {
            // Проверяем, занята ли ячейка
            if (table.isOccupied(index)) {
                const KeyValuePair<Key, Value>& pair = table.getListAtIndex(index);
                if (pair.key == key) {
                    return &pair.value;
                }
            }
            index = (index + 1) % table.capacity();
        } while (index != originalIndex);

        return nullptr;
    }


    // Проверка наличия ключа в словаре
    bool contains(const Key& key) const {
        // Создаем временную пару с фиктивным значением для поиска
        return table.contains(KeyValuePair<Key, Value>(key, Value()));
    }

    typename HashTable<KeyValuePair<Key, Value>>::iterator begin() {
        return table.begin();
    }

    typename HashTable<KeyValuePair<Key, Value>>::iterator end() {
        return table.end();
    }

    // Статический метод для тестирования
    static void testDictionary() {
        Dictionary<int, string> dict(10);

        // Тестирование insert
        dict.insert(1, "one");
        dict.insert(2, "two");
        dict.insert(11, "eleven"); // Проверка коллизии

        // Тестирование operator[]
        assert(dict[1] == "one");
        assert(dict[2] == "two");
        assert(dict[11] == "eleven");
        bool caught = false;
        try {
            dict[3]; // Ключ не существует
        }
        catch (const runtime_error& error) {
            caught = true;
        }
        assert(caught);


        // Тестирование contains
        assert(dict.contains(1));
        assert(dict.contains(2));
        assert(dict.contains(11));
        assert(!dict.contains(3));

        // Тестирование find
        assert(*dict.find(1) == "one");
        assert(*dict.find(2) == "two");
        assert(*dict.find(11) == "eleven");
        assert(dict.find(3) == nullptr);

        const Dictionary<int, string>& constDict = dict;
        assert(*constDict.find(1) == "one"); // Тестирование const версии find


        // Тестирование erase
        dict.erase(1);
        assert(!dict.contains(1));
        assert(dict.find(1) == nullptr);
        assert(dict.contains(2));
        assert(dict.contains(11));



        // Тестирование insert после erase
        dict.insert(1, "one_again");
        assert(dict.contains(1));
        assert(*dict.find(1) == "one_again");




        cout << "Все тесты пройдены!" << endl;
    }

};