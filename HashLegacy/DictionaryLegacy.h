#include "HashLegacy.h"
#include "PairLegacy.h"
//HashTable<Key>::
using namespace std;

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
    // Конструктор словаря. 47 -- простое число, число элементов по умолчанию
    Dictionary(size_t capacity = 47, function<size_t(const KeyValuePair<Key, Value>&)> hashFunction = [](const KeyValuePair<Key, Value>& p) { return HashTable<Key>::defaultHash(p.key); }, double maxLoadFactor = 0.7)
        : table(capacity, hashFunction, maxLoadFactor) {}

    // Вставка пары ключ-значение в словарь
    void insert(const Key& key, const Value& value) {
        KeyValuePair<Key, Value> tempPair(key, value);
        if (contains(key))
        {
            size_t index = table.hash(tempPair) % table.capacity();
            size_t originalIndex = index;

            do {
                // Проверяем, занята ли ячейка
                if (table.isOccupied(index)) {
                    KeyValuePair<Key, Value>& pair = table.getListAtIndex(index);
                    if (pair.key == key) {
                        table.getListAtIndex(index) = tempPair;
                    }
                }
                index = (index + 1) % table.capacity();
            } while (index != originalIndex);
        }
        else
        {
        table.insert(tempPair);
        }
    }

    // Удаление пары ключ-значение из словаря.
    void erase(const Key& key) {
        table.erase(KeyValuePair<Key, Value>(key, Value())); // V() создает дефолтное значение для V, необходимо для поиска и удаления
    }


    // Получение значения по ключу. Бросает исключение runtime_error, если ключ не найден
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

    // Получение значения по ключу (константная версия). Бросает исключение runtime_error, если ключ не найден
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

    // Поиск значения по ключу. Возвращает nullptr, если значение не найдено
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

    // Поиск значения по ключу (константная версия). Возвращает nullptr, если значение не найдено
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
    //Итератор, указывающий на начало словаря
    typename HashTable<KeyValuePair<Key, Value>>::iterator begin() {
        return table.begin();
    }
    //Итератор, указывающий на конец словаря
    typename HashTable<KeyValuePair<Key, Value>>::iterator end() {
        return table.end();
    }

    //Итератор, указывающий на начало словаря (константная версия)
    typename HashTable<KeyValuePair<Key, Value>>::iterator begin() const {
        return table.begin(); // или table.cbegin(), если HashTable его предоставляет
    }

    //Итератор, указывающий на конец словаря (константная версия)
    typename HashTable<KeyValuePair<Key, Value>>::iterator end() const {
        return table.end(); // или table.cend(), если HashTable его предоставляет
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




        cout << "All tests passed successfully!" << endl;
    }

};