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

using namespace std;

std::wstring clear_and_lowercase(const std::wstring& input) {
    std::wstring result;
    std::locale loc("ru_RU.UTF-8"); //  Локаль для корректной работы с кириллицей

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

//// Функция для очистки слова от знаков препинания и преобразования к нижнему регистру с помощью regex
//string clean_word(string word) {
//    // Удаляем все неалфанумерические символы
//    regex non_alphanumeric("[^a-zA-Z-а-яА-Я]"); // Добавлена поддержка кириллицы
//    word = regex_replace(word, non_alphanumeric, "");
//
//    // Преобразуем к нижнему регистру
//    transform(word.begin(), word.end(), word.begin(), ::tolower);
//
//    return word;
//}
void generatePlantUMLGraph(const std::vector<KeyValuePair<std::string, int>>& data, const std::string& filename, double stop=0.2) {
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

void process_file(const string& filename) {
    ifstream file(filename);

    // Если файл не существует, создаем его и завершаем работу
    if (!file.is_open()) {
        ofstream outfile(filename);
        if (!outfile.is_open()) {
            cerr << "Ошибка создания файла: " << filename << endl;
        }
        return;
    }

    Dictionary<string, int> word_counts(100);

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        while (ss >> word) {
            word = clean_word(word); // Очищаем слово
            if (!word.empty()) { // Проверяем, что слово не пустое после очистки
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

    // Сортировка словаря по значению (в порядке убывания)
    vector<KeyValuePair<string, int>> sorted_word_counts;
    for (const auto& pair : word_counts) {
        sorted_word_counts.push_back(pair);
    }

    sort(sorted_word_counts.begin(), sorted_word_counts.end(), [](const auto& a, const auto& b) {
        return a.value > b.value;
        });

    // Создаем файл данных
    std::ofstream dataFile("data.txt");
    for (const auto& pair : sorted_word_counts) {
        dataFile << pair.key << " " << pair.value << "\n";
    }
    dataFile.close();

    generatePlantUMLGraph(sorted_word_counts, "chart.txt");

    // Вывод отсортированного словаря
    for (const auto& pair : sorted_word_counts) {
        cout << pair.key << ": " << pair.value << endl;
    }
}



int main() {
    // Создание словаря
    Dictionary<string, int> dict(10);
    Dictionary<int, string>::testDictionary();
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

    //process_file("cipfile.txt");


    return 0;
}