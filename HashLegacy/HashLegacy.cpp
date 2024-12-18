// HashLegacy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "HashLegacy.h"
#include "DictionaryLegacy.h"
#include "SetLegacy.h"
#include <utility>
#include <cctype>
#include <regex>
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>

/*
ХТ:
1. Код: Таблица рехэшируется, если слишком мало элементов
2. Док: Исправлено, написано о бросаемых исключениях
3. Тесты: Тесты с коллизиями и изменением размера хэша
Словарь:
1. Код: Обновляет значение в словаре, если оно уже есть
2. Документация: Обновлена, написано о бросаемых исключениях
Множество:
1. Документация: Обновлена
2. Тесты:
3. Итератор: Имеется
4. Гит: Имеется
Закон Ципфа:
1. Разделен код на отдельные части

*/



using namespace std;

void generatePlantUMLGraph(const std::vector<KeyValuePair<std::string, size_t>>& data, const std::string& filename, double stop = 0.2) {
    std::ofstream outFile(filename);

    // Проверяем, успешно ли открыт файл
    if (!outFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи." << std::endl;
        return;
    }

    // Записываем заголовок и настройки для PlantUML
    outFile << "import matplotlib.pyplot as plt\n";
    outFile << "import numpy as np\n";
    outFile << "import seaborn # pip install seaborn\n";
    outFile << "plt.figure( figsize=(150, 100) )\n";
    outFile << "plt.title(\"Закон Ципфа\")\n";


    stringstream words;
    stringstream numbers;
    words << "words =[";
    numbers << "numbers =[";

    size_t i = 0;

    for (const auto& pair : data) {
        words << "\"" << pair.key << "\", ";
        numbers << pair.value << ", ";
        i++;
        if (i / data.size() > stop)
            break;
    }
    string words_s = words.str();
    words_s.erase(words_s.end() - 2, words_s.end());
    words.str("");
    words << words_s;

    string numbers_s = numbers.str();
    numbers_s.erase(numbers_s.end() - 2, numbers_s.end());
    numbers.str("");
    numbers << numbers_s;
    words << "]";
    numbers << "]";


    outFile << words.str() << "\n";
    outFile << numbers.str() << "\n";

    outFile << "plt.plot(words, numbers, marker=\'o\', linestyle=\'-\')\n";


    outFile << "plt.grid(True)\n";
    outFile << "plt.xlabel('Ранг слова')\n";
    outFile << "plt.xticks(rotation=90) \n";
    outFile << "plt.ylabel('Частота встречи')\n";
    outFile << "import seaborn # pip install seaborn\n";
    outFile << "plt.legend(loc = 'best')\n";
    outFile << "plt.savefig('plot.svg', format='svg')\n";
    outFile << "plt.show()\n";



    // Закрываем файл
    outFile.close();
}

std::wstring clear_and_lowercase(const std::wstring& input) {
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");

    for (wchar_t c : input) {
        if (std::isalpha(c, loc)) {
            result += std::tolower(c, loc);
        }
    }
    return result;
}

std::string clean_word(const std::string& input) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(input);
    std::wstring cleaned_wide_string = clear_and_lowercase(wide_string);
    return converter.to_bytes(cleaned_wide_string);
}


Dictionary<string, size_t> load_word_counts_from_file(const string& filename) {
    ifstream file(filename);

    // Если файл не существует, создаем пустой словарь
    if (!file.is_open()) {
        cerr << "Файл не найден: " << filename << endl;
        return Dictionary<string, size_t>(100); // Возвращаем пустой словарь
    }


    Dictionary<string, size_t> word_counts(100);

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        while (ss >> word) {
            word = clean_word(word);
            if (!word.empty()) {
                if (word_counts.contains(word)) {
                    word_counts[word]++;
                }
                else {
                    word_counts.insert(word, 1);
                }
            }
        }
    }
    file.close();
    return word_counts;
}


vector<KeyValuePair<string, size_t>> sort_word_counts(const Dictionary<string, size_t> & word_counts) {
    vector<KeyValuePair<string, size_t>> sorted_word_counts;
    for (const auto& pair : word_counts) {
        sorted_word_counts.push_back(pair);
    }

    sort(sorted_word_counts.begin(), sorted_word_counts.end(), [](const auto& a, const auto& b) {
        return a.value > b.value;
        });
    return sorted_word_counts;
}


void save_word_counts_to_file(const vector<KeyValuePair<string, size_t>>& sorted_word_counts, const std::string& filename) {
    ofstream dataFile(filename);
    if (!dataFile.is_open()) {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
        return;
    }

    for (const auto& pair : sorted_word_counts) {
        dataFile << pair.key << " " << pair.value << "\n";
    }
    dataFile.close();
}



void process_file(const string& filename) {
    Dictionary<string, size_t> word_counts = load_word_counts_from_file(filename);
    vector<KeyValuePair<string, size_t>> sorted_word_counts = sort_word_counts(word_counts);
    save_word_counts_to_file(sorted_word_counts, "data.txt");
    generatePlantUMLGraph(sorted_word_counts, "chart.txt");

    for (const auto& pair : sorted_word_counts) {
        cout << pair.key << ": " << pair.value << endl;
    }
}



int main() {
    // Создание словаря
    Dictionary<string, int> dict(10);
    HashTable<int>::testAllMethods();
    Set<int>::testAllMethods();
    Dictionary<int, string>::testDictionary();
    return 0;
}