#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>

using namespace std;

/* Структура std::hash является шаблонным классом, который предоставляет хеш-функции для различных типов данных.
 * Она используется для вычисления хеш-значений, которые необходимы для хеш-таблиц и других ассоциативных контейнеров.
 *
 * std::hash определен как шаблонный класс, что позволяет использовать его для различных типов данных.
 * Например: std::hash<int>, std::hash<std::string> и т.д.
 *
 *   Требования к пользовательским типам:
 *    - Для использования пользовательского типа необходимо определить специализацию std::hash для этого типа.
 *    - Специализация должна перегружать оператор вызова функции и возвращать хеш-значение.
 *    - Шаблон специализации для пользовательского класса:
 *
 *      namespace std {
 *          template <>
 *          struct hash<ClassName> {
 *              size_t operator()(const ClassName& class) const {
 *                  size_t hash = 5381;
 *                  hash = ((hash << 5) + hash) + std::hash<int>{}(class.field1);
 *                  ...
 *                  hash = ((hash << 5) + hash) + std::hash<std::string>{}(class.fieldN);
 *                  return hash;
 *              }
 *          };
 *      }
 */


 // Класс-пример для тестирования
 // Класс Юзера, содержащий ID и Имя
class User {
private:
    int id;
    string name;

public:
    User(int id, const std::string& name) : id(id), name(name) {}

    int getId() const { return id; }
    std::string getName() const { return name; }

    // Оператор сравнения для проверки равенства двух объектов User
    bool operator==(const User& other) const {
        return id == other.id && name == other.name;
    }

    // Дружественная функция для хеш-функции, позволяет функции использовать приватные поля класса напрямую
    friend struct std::hash<User>;
};

// Хеш-функция для класса User, определённая в пространстве имён std
namespace std {
    template <>
    struct hash<User> {
        size_t operator()(const User& user) const {
            size_t hash = 5381;
            hash = ((hash << 5) + hash) + std::hash<int>{}(user.id);
            hash = ((hash << 5) + hash) + std::hash<std::string>{}(user.name);
            return hash;
        }
    };
}


template <typename Key>
size_t djb2Hash(const Key& key) {
    size_t hash = 5381;
    auto key_hash = std::hash<Key>{}(key);
    hash = ((hash << 5) + hash) + key_hash;
    return hash;
}



template <typename Key>
 size_t fnv1aHash(const Key& key) {
    size_t hash = 2166136261;
    auto key_hash = std::hash<Key>{}(key);
    hash = (hash ^ key_hash) * 16777219;
    return hash;
}
template <typename Key>
size_t murmurHash(const Key& key) {
    size_t hash = 2166136261;
    auto key_hash = std::hash<Key>{}(key);
    hash = (hash ^ key_hash) * 0x5bd1e995;
    return hash;
}

// Хеш-функция, использующая квадратичную функцию
template <typename Key>
size_t quadraticHash(const Key& value) {
    return value * value + 3 * value + 7;
}



// Шаблонный класс хеш-таблицы
template <typename Key>
class HashTable {
private:
    // Вектор списков для хранения ключей
    vector<list<Key>> table;
    // Хеш-функция
    function<size_t(const Key&)> hashFunction;
    // Количество ключей в таблице
    size_t _size;
    // Коэффициент загрузки -- степень загруженности таблциы
    double loadFactor;
    // Максимальный коэффициент загрузки
    double maxLoadFactor;

    // Функция линейного зондирования
    size_t linearProbing(size_t index, size_t capacity) {
        return (index + 1) % capacity;
    }

public:


    // Хеш-функция по умолчанию
    static size_t defaultHash(const Key& value) {
        // Простая хеш-функция, которая возвращает значение ключа
        return fnv1aHash(value);
    }


    HashTable(size_t capacity, function<size_t(const Key&)> hashFunction = defaultHash, double maxLoadFactor = 0.7)
        : table(capacity), hashFunction(hashFunction), _size(0), loadFactor(0.0), maxLoadFactor(maxLoadFactor) {}

    // Деструктор хеш-таблицы
    ~HashTable() {}

    // Вставка ключа в таблицу
    // Сложность: O(1) в среднем случае, O(n) в худшем случае
    void insert(const Key& key) {
        // Вычисление индекса по хеш-функции
        size_t index = hashFunction(key) % table.size();
        // Проверка на коллизию
        if (find(table[index].begin(), table[index].end(), key) != table[index].end()) {
            // Решение коллизии - линейное зондирование
            index = linearProbing(index, table.size());
        }
        // Вставка ключа в список по индексу
        table[index].push_back(key);
        _size++;
        loadFactor = (double)_size / table.size();
        // Перехеширование при превышении максимального коэффициента загрузки
        if (loadFactor > maxLoadFactor) {
            rehash();
        }
    }
    //Хэширование
    size_t hash(const Key& key) const {
        return hashFunction(key);
    }

    list<Key>& getBucket(size_t index) {
        if (index >= table.size()) {
            throw std::out_of_range("Index out of range");
        }
        return table[index];
    }

    const list<Key>& getBucket(size_t index) const {
        if (index >= table.size()) {
            throw std::out_of_range("Index out of range");
        }
        return table[index];
    }

    // Удаление ключа из таблицы
    // Сложность: O(1) в среднем случае, O(n) в худшем случае
    void erase(const Key& key) {// Вычисление индекса по хеш-функции
        size_t index = hashFunction(key) % table.size();
        // Проверка на коллизию
        auto it = find(table[index].begin(), table[index].end(), key);
        if (it != table[index].end()) {
            // Удаление ключа из списка по индексу
            table[index].erase(it);
            _size--;
            loadFactor = (double)_size / table.size();
        }
    }

    // Проверка наличия ключа в таблице
    // Сложность: O(1) в среднем случае, O(n) в худшем случае
    bool contains(const Key& key) const {
        // Вычисление индекса по хеш-функции
        size_t index = hashFunction(key) % table.size();
        // Проверка на коллизию
        return find(table[index].begin(), table[index].end(), key) != table[index].end();
    }

    // Перехеширование при превышении максимального коэффициента загрузки
    void rehash() {
        // Создание новой таблицы с удвоенной емкостью
        vector<list<Key>> newTable(table.size() * 2);
        // Перехеширование ключей в новую таблицу
        for (const auto& list : table) {
            for (const auto& key : list) {
                size_t index = hashFunction(key) % newTable.size();
                newTable[index].push_back(key);
            }
        }
        // Замена старой таблицы на новую
        table = newTable;
        loadFactor = (double)_size / table.size();
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return table.size();
    }

    // Итератор для хеш-таблицы
    class iterator {
    private:
        // Указатель на текущий список
        typename vector<list<Key>>::iterator currentList;
        // Указатель на текущий элемент в списке
        typename list<Key>::iterator currentElement;
        // Указатель на конец таблицы
        typename vector<list<Key>>::iterator endList;

    public:
        // Конструктор итератора
        iterator(typename vector<list<Key>>::iterator begin, typename vector<list<Key>>::iterator end)
            : currentList(begin), endList(end) {
            // Находим первый непустой список
            while (currentList != endList && currentList->empty()) {
                ++currentList;
            }
            if (currentList != endList) {
                currentElement = currentList->begin();
            }
        }

        // Оператор инкремента
        iterator& operator++() {
            // Переход к следующему элементу в списке
            if (currentElement != currentList->end()) {
                ++currentElement;
            }
            // Если достигли конца списка, переходим к следующему списку
            if (currentElement == currentList->end()) {
                ++currentList;
                // Находим следующий непустой список
                while (currentList != endList && currentList->empty()) {
                    ++currentList;
                }
                if (currentList != endList) {
                    currentElement = currentList->begin();
                }
            }
            return *this;
        }

        // Оператор разыменования
        Key& operator*() const {
            return *currentElement;
        }

        // Оператор сравнения
        bool operator!=(const iterator& other) const {
            if (currentList == endList && other.currentList == endList) {
                return false; // Оба итератора достигли конца хеш-таблицы
            }
            if (currentList == endList || other.currentList == endList) {
                return true; // Один из итераторов достиг конца хеш-таблицы
            }
            return currentList != other.currentList || currentElement != other.currentElement;
        }
    };

    // Начало итератора
    iterator begin() {
        return iterator(table.begin(), table.end());
    }

    // Конец итератора
    iterator end() {
        return iterator(table.end(), table.end());
    }
    // Метод очистки значений хэш-таблицы
    void clear() {
        // Очищаем все списки в таблице
        for (auto& list : table) {
            list.clear();
        }
        // Сбрасываем размер таблицы и коэффициент загрузки
        _size = 0;
        loadFactor = 0.0;
    }

    // Статический метод для тестирования всех методов класса
    static void testAllMethods() {
        HashTable<int> hashTable(10);

        // Тестируем insert
        for (int i = 0; i < 100; i++) {
            hashTable.insert(i);
            assert(hashTable.contains(i));
        }

        // Тестируем contains
        for (int i = 0; i < 100; i++) {
            assert(hashTable.contains(i));
        }
        assert(!hashTable.contains(100));
        assert(!hashTable.contains(101));

        // Тестируем erase
        for (int i = 0; i < 50; i++) {
            hashTable.erase(i);
            assert(!hashTable.contains(i));
        }
        for (int i = 50; i < 100; i++) {
            assert(hashTable.contains(i));
        }

        // Тестируем rehash
        for (int i = 0; i < 1000; i++) {
            hashTable.insert(i);
        }
        for (int i = 0; i < 1000; i++) {
            assert(hashTable.contains(i));
        }

        // Тестируем итератор
        int count = 0;
        for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
            count++;
        }
        assert(count == 1050);


        // Тестируем insert с одинаковыми ключами
        hashTable.insert(10);
        hashTable.insert(10);
        assert(hashTable.contains(10));

        // Тестируем erase с несуществующим ключом
        hashTable.erase(2000);
        assert(!hashTable.contains(2000));

        // Тестируем contains с несуществующим ключом
        assert(!hashTable.contains(2000));

        // Тестируем rehash с пустой таблицей
        HashTable<int> emptyHashTable(10);
        emptyHashTable.rehash();
        assert(emptyHashTable.size() == 0);

        // Тестируем хеш-таблицу с строками
        HashTable<string> stringHashTable(10);

        // Тестируем insert для строк
        stringHashTable.insert("hello");
        stringHashTable.insert("world");
        stringHashTable.insert("test");
        stringHashTable.insert("example");

        // Тестируем contains для строк
        assert(stringHashTable.contains("hello"));
        assert(stringHashTable.contains("world"));
        assert(stringHashTable.contains("test"));
        assert(stringHashTable.contains("example"));
        assert(!stringHashTable.contains("not_in_table"));

        // Тестируем erase для строк
        stringHashTable.erase("hello");
        assert(!stringHashTable.contains("hello"));
        assert(stringHashTable.contains("world"));
        assert(stringHashTable.contains("test"));
        assert(stringHashTable.contains("example"));

        // Тестируем rehash для строк
        for (int i = 0; i < 100; i++) {
            stringHashTable.insert("key" + to_string(i));
        }
        for (int i = 0; i < 100; i++) {
            assert(stringHashTable.contains("key" + to_string(i)));
        }

        // Тестируем итератор для строк
        count = 0;
        for (auto it = stringHashTable.begin(); it != stringHashTable.end(); ++it) {
            count++;
        }
        assert(count == 103); // 100 ключей "key0".."key99" + "world" + "test" + "example"

        // Тестируем insert с одинаковыми ключами для строк
        stringHashTable.insert("test");
        stringHashTable.insert("test");
        assert(stringHashTable.contains("test"));

        // Тестируем erase с несуществующим ключом для строк
        stringHashTable.erase("not_in_table");
        assert(!stringHashTable.contains("not_in_table"));

        // Тестируем contains с несуществующим ключом для строк
        assert(!stringHashTable.contains("not_in_table"));

        // Тестируем rehash с пустой таблицей для строк
        HashTable<string> emptyStringHashTable(10);
        emptyStringHashTable.rehash();
        assert(emptyStringHashTable.size() == 0);

        // Тестируем хеш-таблицу с ключами-пользовательскими классами
        HashTable<User> userHashTable(10);

        // Тестируем insert для пользовательских классов
        User user1(1, "Alice");
        User user2(2, "Bob");
        User user3(3, "Charlie");
        userHashTable.insert(user1);
        userHashTable.insert(user2);
        userHashTable.insert(user3);

        // Тестируем contains для пользовательских классов
        assert(userHashTable.contains(user1));
        assert(userHashTable.contains(user2));
        assert(userHashTable.contains(user3));
        assert(!userHashTable.contains(User(4, "David")));

        // Тестируем erase для пользовательских классов
        userHashTable.erase(user1);
        assert(!userHashTable.contains(user1));
        assert(userHashTable.contains(user2));
        assert(userHashTable.contains(user3));

        // Тестируем rehash для пользовательских классов
        for (int i = 0; i < 100; i++) {
            userHashTable.insert(User(i + 4, "User" + to_string(i + 4)));
        }
        for (int i = 0; i < 100; i++) {
            assert(userHashTable.contains(User(i + 4, "User" + to_string(i + 4))));
        }

        // Тестируем итератор для пользовательских классов
        count = 0;
        for (auto it = userHashTable.begin(); it != userHashTable.end(); ++it) {
            count++;
        }
        assert(count == 102); // 100 пользователей + "Bob" + "Charlie"

        // Тестируем insert с одинаковыми ключами для пользовательских классов
        userHashTable.insert(user2);
        userHashTable.insert(user2);
        assert(userHashTable.contains(user2));

        // Тестируем erase с несуществующим ключом для пользовательских классов
        userHashTable.erase(User(1000, "NonExistent"));
        assert(!userHashTable.contains(User(1000, "NonExistent")));

        // Тестируем contains с несуществующим ключом для пользовательских классов
        assert(!userHashTable.contains(User(1000, "NonExistent")));

        // Тестируем rehash с пустой таблицей для пользовательских классов
        HashTable<User> emptyUserHashTable(10);
        emptyUserHashTable.rehash();
        assert(emptyUserHashTable.size() == 0);

        cout << "All tests passed successfully!" << endl;
    }

};
