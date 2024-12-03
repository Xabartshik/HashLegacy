// HashLegacy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "HashLegacy.h"
#include "DictionaryLegacy.h"
#include "SetLegacy.h"
#include <utility>
using namespace std;

int main() {
    // Создание словаря
    Dictionary<string, int> dict(10);
    //dict.put("apple", 5);
    //dict.put("banana", 7);

    dict.insert("apple", 5);
    dict.insert("banana", 7);
    int value = dict["apple"];
    // Получение значения по ключу
    cout << value << endl; // Вывод: 5
    for (auto const& val : dict) {
        cout << val.key << ": " << val.value << endl;
    }
    
    //// Проверка наличия ключа
    cout << dict.contains("orange") << endl; // Вывод: 0

    cout << "Set" << endl;
    // Создание множества
    Set<int> set(10);
    set.insert(5);
    set.insert(5);
    set.insert(5);
    set.insert(7);
    for (auto const& val : set) {
        cout << val << endl;
    }
    // Проверка наличия ключа
    cout << set.contains(5) << endl; // Вывод: 1

    return 0;
}