
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>

using namespace std;


// Шаблонный класс хеш-таблицы
template <typename Key>
class HashLegacy {
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

    static size_t djb2Hash(const Key& key) {
        size_t hash = 5381;
        auto key_hash = std::hash<Key>{}(key);
        hash = ((hash << 5) + hash) + key_hash;
        return hash;
    }


    static size_t fnv1aHash(const Key& key) {
        size_t hash = 2166136261;
        auto key_hash = std::hash<Key>{}(key);
        hash = (hash ^ key_hash) * 16777219;
        return hash;
    }

    static size_t murmurHash(const Key& key) {
        size_t hash = 2166136261;
        auto key_hash = std::hash<Key>{}(key);
        hash = (hash ^ key_hash) * 0x5bd1e995;
        return hash;
    }

    // Хеш-функция по умолчанию
    static size_t defaultHash(const Key& value) {
        // Простая хеш-функция, которая возвращает значение ключа
        return fnv1aHash(value);
    }

    // Хеш-функция, использующая квадратичную функцию
    size_t quadraticHash(const Key& value) {
        return value * value + 3 * value + 7;
    }
 

    HashLegacy(size_t capacity, function<size_t(const Key&)> hashFunction = defaultHash, double maxLoadFactor = 0.7)
        : table(capacity), hashFunction(hashFunction), _size(0), loadFactor(0.0), maxLoadFactor(maxLoadFactor) {}

    // Деструктор хеш-таблицы
    ~HashLegacy() {}

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
        HashLegacy<int> hashTable(10);

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
        HashLegacy<int> emptyHashTable(10);
        emptyHashTable.rehash();
        assert(emptyHashTable.size() == 0);
        cout << "All tests passed successfully!" << endl;
    }

};
