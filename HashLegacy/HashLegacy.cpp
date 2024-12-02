// HashLegacy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "HashLegacy.h"

using namespace std;

int main() {
    HashTable<int>::testAllMethods();
    // Создание хеш-таблицы с хэш-функцией по умолчанию
    HashTable<int> hashTableDefault(10);
    hashTableDefault.insert(5);
    hashTableDefault.insert(10);
    hashTableDefault.insert(15);

    // Проверка наличия ключа в таблице
    std::cout << "Хэш-таблица с хэш-функцией по умолчанию:" << std::endl;
    std::cout << "Содержит ключ 5: " << (hashTableDefault.contains(5) ? "Да" : "Нет") << std::endl;
    std::cout << "Содержит ключ 10: " << (hashTableDefault.contains(10) ? "Да" : "Нет") << std::endl;
    std::cout << "Содержит ключ 15: " << (hashTableDefault.contains(15) ? "Да" : "Нет") << std::endl;




    return 0;
}