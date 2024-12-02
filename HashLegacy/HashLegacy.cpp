// HashLegacy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "HashLegacy.h"

using namespace std;

int main() {
    HashLegacy<int>::testAllMethods();
    // Создание хеш-таблицы с хэш-функцией по умолчанию
    HashLegacy<string> hashTableDefault(10, HashLegacy<string>::djb2Hash);
    hashTableDefault.insert("ФФ");
    hashTableDefault.insert("!");
    hashTableDefault.insert("XCFVGBHNJM");

    // Проверка наличия ключа в таблице
    std::cout << "Хэш-таблица с хэш-функцией по умолчанию:" << std::endl;
    std::cout << "Содержит ключ 5: " << (hashTableDefault.contains("ФФ") ? "Да" : "Нет") << std::endl;
    std::cout << "Содержит ключ 10: " << (hashTableDefault.contains("!") ? "Да" : "Нет") << std::endl;
    std::cout << "Содержит ключ 15: " << (hashTableDefault.contains("FGHJK") ? "Да" : "Нет") << std::endl;




    return 0;
}