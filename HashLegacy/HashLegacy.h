#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>

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
    User() : id(0), name("") {}
    User(int id, const std::string& name) : id(id), name(name) {}

    int getId() const { return id; }
    std::string getName() const { return name; }

    // Оператор сравнения для проверки равенства двух объектов User
    bool operator==(const User& other) const {
        return id == other.id && name == other.name;
    }

    // Оператор сравнения для проверки равенства двух объектов User
    bool operator!=(const User& other) const {
        return !(id == other.id && name == other.name);
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

//Хэш-функция djb2
template <typename Key>
size_t djb2Hash(const Key& key) {
    size_t hash = 5381;
    auto key_hash = std::hash<Key>{}(key);
    hash = ((hash << 5) + hash) + key_hash;
    return hash;
}


//Хэш-функция fnv1a
template <typename Key>
size_t fnv1aHash(const Key& key) {
    size_t hash = 2166136261;
    auto key_hash = std::hash<Key>{}(key);
    hash = (hash ^ key_hash) * 16777219;
    return hash;
}
//Хэш-функция murmur
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

//Хэш-функция k0syak
template <typename Key>
size_t k0syakHash(const Key& key) {
    size_t hash = 123;
    return hash;
}


// Шаблонный класс хеш-таблицы
template <typename Key>
class HashTable {
private:
    // Вектор ключей для хранения данных
    vector<Key> table;
    // Вектор флагов, указывающих, занята ли ячейка
    vector<bool> occupied;
    // Хеш-функция
    function<size_t(const Key&)> hashFunction;
    // Количество ключей в таблице
    size_t _size;
    // Коэффициент загрузки -- степень загруженности таблицы
    double loadFactor;
    // Максимальный коэффициент загрузки
    double maxLoadFactor;
    // Минимальный коэффициент загрузки
    double minLoadFactor;
public:
    // Хеш-функция по умолчанию
    static size_t defaultHash(const Key& value) {
        // Простая хеш-функция, которая возвращает значение ключа
        return fnv1aHash(value);
    }

    // Конструктор хеш-таблицы
    HashTable(size_t capacity, function<size_t(const Key&)> hashFunction = defaultHash, double maxLoadFactor = 0.7, double minLoadFactor = 0.2)
        : table(capacity), occupied(capacity, false), hashFunction(hashFunction), _size(0), loadFactor(0.0), maxLoadFactor(maxLoadFactor), minLoadFactor(minLoadFactor) {}

    // Деструктор хеш-таблицы
    ~HashTable() {}

    // Вставка ключа в таблицу
        // Сложность: O(1) в среднем случае, O(n) в худшем случае
    void insert(const Key& key) {
        size_t index = hashFunction(key) % table.size();

        // Линейное зондирование для разрешения коллизий
        while (occupied[index]) {
            index = (index + 1) % table.size();
        }

        table[index] = key;
        occupied[index] = true;
        _size++;
        loadFactor = (double)_size / table.size();

        if (loadFactor > maxLoadFactor) {
            rehash();
        }
    }


    //Функция хэширования через функцию хэш-таблицы
    size_t hash(const Key& key) const {
        return hashFunction(key);
    }

    // Удаление ключа из таблицы
        // Сложность: O(1) в среднем случае, O(n) в худшем случае
    void erase(const Key& key) {
        size_t index = hash(key) % table.size();
        size_t start_index = index;

        // Линейное зондирование для поиска ключа
        while (table[index] != key) {
            index = (index + 1) % table.size();
            if (start_index == index)
                break;
        }

        // Если ключ найден, удаляем его, устанавливая флаг occupied в false
        if (occupied[index] && table[index] == key) {
            occupied[index] = false;
            table[index] = Key();
            _size--;
            loadFactor = (double)_size / table.size();
            if (loadFactor < minLoadFactor)
            {
                rehash();
            }


        }
    }

    // Проверка наличия ключа в таблице
        // Сложность: O(1) в среднем случае, O(n) в худшем случае
    bool contains(const Key& key) const {
        size_t index = hash(key) % table.size();
        size_t start_index = index;
        // Линейное зондирование для поиска ключа
        while (table[index] != key) {
            index = (index + 1) % table.size();
            if (start_index == index)
                break;
        }

        return occupied[index] && table[index] == key;
    }
    //Получить значение ячейки по индексу. Бросает исключение out_of_range, если индекс указан неверно
    const Key& getListAtIndex(size_t index) const {
        if (index >= table.size()) {
            throw out_of_range("Index out of range");
        }
        return table[index];
    }
    //Получить значение ячейки по индексу. Бросает исключение out_of_range, если индекс указан неверно
    Key& getListAtIndex(size_t index) {
        if (index >= table.size()) {
            throw out_of_range("Index out of range");
        }
        return table[index];
    }
    //Получить занятость ячейки по индексу. Бросает исключение out_of_range, если индекс указан неверно
    bool isOccupied(size_t index) const {
        if (index >= table.size()) {
            throw out_of_range("Index out of range");
        }
        return occupied[index];
    }




    // Перехеширование при превышении максимального коэффициента загрузки
    void rehash() {
        size_t oldSize = table.size();
        vector<Key> oldTable = table;
        vector<bool> oldOccupied = occupied;

        if (loadFactor > maxLoadFactor)
        {
            table.assign(oldSize * 2, Key());
            occupied.assign(oldSize * 2, false);
            _size = 0;
            loadFactor = 0.0;

            for (size_t i = 0; i < oldSize; ++i) {
                if (oldOccupied[i]) {
                    insert(oldTable[i]);
                }
            }
        }
        else
        {
            table.assign(oldSize / 2, Key());
            occupied.assign(oldSize / 2, false);
            _size = 0;
            loadFactor = 0.0;

            for (size_t i = 0; i < oldSize; ++i) {
                if (oldOccupied[i]) {
                    insert(oldTable[i]);
                }
            }

        }


    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return table.size();
    }


    class iterator {
    private:
        const std::vector<Key>* table; // Теперь храним указатель на table
        typename std::vector<Key>::const_iterator current; // const_iterator
        typename std::vector<Key>::const_iterator end; // const_iterator
        const std::vector<bool>* occupied;


    public:
        iterator(const std::vector<Key>* table, typename std::vector<Key>::const_iterator begin,  // const_iterator
            typename std::vector<Key>::const_iterator end, const std::vector<bool>* occupied)
            : table(table), current(begin), end(end), occupied(occupied) {
            // Находим первый занятый элемент
            while (current != end && !(*occupied)[std::distance(table->begin(), current)]) {
                ++current;
            }
        }

        iterator& operator++() {
            ++current;
            while (current != end && !(*occupied)[std::distance(table->begin(), current)]) {
                ++current;
            }
            return *this;
        }

        const Key& operator*() const { // const Key&
            return *current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };
    //Итератор на начало таблицы
    iterator begin() {
        return iterator(&table, table.begin(), table.end(), &occupied);
    }
    //Итератор на конец таблицы
    iterator end() {
        return iterator(&table, table.end(), table.end(), &occupied);
    }

    // Константные версии begin() и end()
    const iterator begin() const {
        return iterator(&table, table.begin(), table.end(), &occupied); 
    }

    const iterator end() const {
        return iterator(&table, table.end(), table.end(), &occupied); 
    }

    // Метод очистки значений хэш-таблицы
    void clear() {
        // Очищаем таблицу
        for (auto& key : table) {
            key = Key();
        }
        for (auto& key : occupied) {
            key = false;
        }
        // Сбрасываем размер таблицы и коэффициент загрузки
        _size = 0;
        loadFactor = 0.0;
    }

    // Статический метод для тестирования всех методов класса
    static void testAllMethods() {
        HashTable<int> hashTable(10);

        // Тестируем insert
        for (int i = 0; i < 10; i++) {
            hashTable.insert(i);
            assert(hashTable.contains(i));
        }

        // Тестируем contains
        for (int i = 0; i < 10; i++) {
            assert(hashTable.contains(i));
        }
        assert(!hashTable.contains(10));
        assert(!hashTable.contains(11));

        // Тестируем erase
        for (int i = 0; i < 5; i++) {
            hashTable.erase(i);
            assert(!hashTable.contains(i));
        }
        for (auto val : hashTable)
        {
            cout << val << endl;
        }
        for (int i = 5; i < 10; i++) {
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
        assert(count == 1005);


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

        //Проверка на разрешение коллизий:
                // Тестируем хеш-таблицу с числами. Хэш-функция максимально ужасная
        HashTable<int> badHashTable(10, k0syakHash<int>);
        for (int i = 0; i < 100; i++) {
            badHashTable.insert(i);
            assert(badHashTable.contains(i));
        }
        // Тестируем erase
        for (int i = 0; i < 95; i++) {
            badHashTable.erase(i);
            assert(!badHashTable.contains(i));
        }
        assert(badHashTable.capacity() == 20);
        //Проверка работоспособности getListAtIndex и isOccupied
        for (int i = 5; i < 10; i++) {
            assert(badHashTable.getListAtIndex(i) == 90 + i);
            assert(badHashTable.isOccupied(i) == true);
        }
        for (int i = 10; i < 20; i++) {
            assert(badHashTable.getListAtIndex(i) == 0);
            assert(badHashTable.isOccupied(i) == false);
        }
        cout << "All tests passed successfully!" << endl;
    }

};
