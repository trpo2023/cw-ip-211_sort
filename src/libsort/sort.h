#include <algorithm>
#include <cctype> // Для функции std::tolower
#include <chrono> // Для работы с датой и временем
#include <ctime>  // Для функции std::mktime
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale> // Для функции std::isspace
#include <random>
#include <regex> // Для работы с регулярными выражениями
#include <string>
#include <vector>

inline void sortVector(
        std::vector<std::string>& vec,
        bool (*sortPredicate)(const std::string&, const std::string&) = nullptr)
{
    if (sortPredicate) {
        std::sort(vec.begin(), vec.end(), sortPredicate);
    } else {
        std::sort(vec.begin(), vec.end());
    }
}

inline bool isFileName(const std::string& str)
{
    std::ifstream file(str);
    return file.good();
}

inline std::string removeLeadingNonPrintableChars(const std::string& str)
{
    std::string cleanStr = str;

    // Удаление ведущих пробелов
    cleanStr.erase(
            cleanStr.begin(),
            std::find_if(cleanStr.begin(), cleanStr.end(), [](unsigned char c) {
                return std::isprint(c);
            }));

    return cleanStr;
}

// -b
inline bool
ignoreLeadingSpaces(const std::string& str1, const std::string& str2)
{
    size_t index1 = str1.find_first_not_of(' ');
    size_t index2 = str2.find_first_not_of(' ');
    return str1.substr(index1) < str2.substr(index2);
}

// -c
inline bool checkSorted(std::vector<std::string>& lines)
{
    if (std::is_sorted(lines.begin(), lines.end())) {
        return true;
    } else {
        return false;
    }
}

// -d
inline bool
dictionaryOrderCompare(const std::string& str1, const std::string& str2)
{
    size_t i = 0;
    size_t j = 0;
    while (i < str1.length() && j < str2.length()) {
        if (std::isalnum(str1[i]) || std::isspace(str1[i])) {
            if (std::isalnum(str2[j]) || std::isspace(str2[j])) {
                if (str1[i] < str2[j])
                    return true;
                if (str1[i] > str2[j])
                    return false;
                i++;
                j++;
            } else {
                j++;
            }
        } else {
            i++;
        }
    }

    return (i == str1.length()) && (j != str2.length());
}

// -f
inline bool
caseInsensitiveCompare(const std::string& str1, const std::string& str2)
{
    size_t length = std::min(str1.length(), str2.length());
    for (size_t i = 0; i < length; ++i) {
        if (std::tolower(str1[i]) != std::tolower(str2[i])) {
            return std::tolower(str1[i]) < std::tolower(str2[i]);
        }
    }
    return str1.length() < str2.length();
}

// -g
inline bool
ignorePunctuationAndSpaces(const std::string& str1, const std::string& str2)
{
    std::locale loc;
    auto isPunctuationOrSpace = [&](char c) {
        return std::ispunct(c, loc) || std::isspace(c, loc);
    };

    auto first1
            = std::find_if_not(str1.begin(), str1.end(), isPunctuationOrSpace);
    auto first2
            = std::find_if_not(str2.begin(), str2.end(), isPunctuationOrSpace);
    return std::lexicographical_compare(first1, str1.end(), first2, str2.end());
}

// -h
inline long long extractValue(const std::string& str)
{
    long long value = std::stoll(str.substr(
            0, str.length() - 1)); // Извлечение числового значения без суффикса
    char suffix = str.back();      // Извлечение суффикса

    switch (suffix) {
    case 'K':
        value *= 1000;
        break;
    case 'M':
        value *= 1000000;
        break;
    case 'G':
        value *= 1000000000;
        break;
    case 'T':
        value *= 1000000000000;
        break;
    }

    return value;
}

inline bool
humanNumericCompare(const std::string& str1, const std::string& str2)
{
    long long value1 = extractValue(str1);
    long long value2 = extractValue(str2);

    return value1 < value2;
}

// --help
inline void displayHelp()
{
    std::cout << "Usage: program [OPTIONS] FILE" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -n                Sort in numeric order" << std::endl;
    std::cout << "  -b                Ignore leading spaces when sorting"
              << std::endl;
    std::cout << "  -d                Sort in dictionary order of digits"
              << std::endl;
    std::cout << "  -f                Case-insensitive sorting" << std::endl;
    std::cout
            << "  -g                Ignore punctuation and spaces when sorting"
            << std::endl;
    std::cout << "  -i                Sort printable characters only"
              << std::endl;
    std::cout << "  -M                Sort by month" << std::endl;
    std::cout << "  -h                Sort in human-readable format"
              << std::endl;
    std::cout << "  -u                Sort and remove duplicate lines"
              << std::endl;
    std::cout << "  -R                Sort randomly" << std::endl;
    std::cout << "  --random-source=FILE" << std::endl;
    std::cout << "                    Get random bytes from FILE" << std::endl;
    std::cout << "  -V                Sort by version numbers" << std::endl;
    std::cout << "  -c                Check if lines are sorted" << std::endl;
    std::cout << "  -kCOLUMN         Sort by the specified column" << std::endl;
    std::cout << "  -r                Reverse the sort order" << std::endl;
    std::cout << "  --help            Display this help and exit" << std::endl;
}

// -i
inline bool isPrintable(const std::string& str1, const std::string& str2)
{
    for (char c : str1) {
        if (!std::isprint(c)) {
            return false;
        }
    }
    return true;
}

// -k
inline void processKFlag(int columnIndex, std::vector<std::string>& lines)
{
    std::sort(
            lines.begin(),
            lines.end(),
            [columnIndex](const std::string& line1, const std::string& line2) {
                std::istringstream iss1(line1);
                std::istringstream iss2(line2);

                std::string word1, word2;
                int i = 1;
                while (i < columnIndex) {
                    if (!(iss1 >> word1) || !(iss2 >> word2))
                        return false; // Ошибка: Недостаточно слов в одной из
                                      // строк, возвращаем false

                    i++;
                }

                // Обработка случая, когда количество слов в строках равно
                // columnIndex
                if (!(iss1 >> word1) || !(iss2 >> word2))
                    return false; // Ошибка: Недостаточно слов в одной из строк,
                                  // возвращаем false

                return word1 < word2;
            });
}

inline void
kSortVector(std::vector<std::string>& lines, std::vector<std::string>& flags)
{
    auto it = std::find_if(
            flags.begin(), flags.end(), [](const std::string& flag) {
                return flag.size() >= 2 && flag.substr(0, 2) == "-k";
            });

    if (it != flags.end()) {
        std::string digit = it->substr(2);

        if (!digit.empty()) {
            int columnIndex = std::stoi(digit);
            processKFlag(columnIndex, lines);
        }
    }
}

// -M
inline bool monthOrderCompare(const std::string& str1, const std::string& str2)
{
    const std::vector<std::string> months
            = {"JAN",
               "FEB",
               "MAR",
               "APR",
               "MAY",
               "JUN",
               "JUL",
               "AUG",
               "SEP",
               "OCT",
               "NOV",
               "DEC"};

    std::string upperStr1 = str1;
    std::transform(
            upperStr1.begin(), upperStr1.end(), upperStr1.begin(), ::toupper);
    std::string upperStr2 = str2;
    std::transform(
            upperStr2.begin(), upperStr2.end(), upperStr2.begin(), ::toupper);

    auto it1 = std::find(months.begin(), months.end(), upperStr1);
    auto it2 = std::find(months.begin(), months.end(), upperStr2);

    if (it1 != months.end() && it2 != months.end()) {
        return it1 - months.begin() < it2 - months.begin();
    } else if (it1 != months.end()) {
        return true;
    } else if (it2 != months.end()) {
        return false;
    } else {
        return str1 < str2;
    }
}

// -n
inline bool isNumeric(const std::string& str)
{
    std::string cleanStr = str;
    cleanStr.erase(
            std::remove_if(
                    cleanStr.begin(),
                    cleanStr.end(),
                    [](unsigned char c) { return !std::isdigit(c); }),
            cleanStr.end());
    return !cleanStr.empty();
}

inline bool
numericOrderCompare(const std::string& str1, const std::string& str2)
{
    if (isNumeric(str1) && isNumeric(str2)) {
        return std::stoi(str1) < std::stoi(str2);
    } else {
        return str1 < str2;
    }
}

// -r
inline void reverseSort(std::vector<std::string>& lines)
{
    std::reverse(lines.begin(), lines.end());
}

// -R
inline void randomSort(std::vector<std::string>& lines)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::shuffle(lines.begin(), lines.end(), gen);
}

// --randon-source
inline void randomSort(std::vector<std::string>& lines, std::mt19937& gen)
{
    std::shuffle(lines.begin(), lines.end(), gen);
}

inline void
randomSort(std::vector<std::string>& lines, std::string& randomSourceFile)
{
    if (randomSourceFile.empty()) {
        std::cerr << "Error: --random-source option is missing the file path."
                  << std::endl;
        return;
    }
    std::ifstream file(randomSourceFile, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open random source file: "
                  << randomSourceFile << std::endl;
        return;
    }
    // Чтение случайных байтов из файла
    std::vector<char> randomBytes(std::istreambuf_iterator<char>(file), {});

    // Преобразование байтов в целочисленный тип
    std::vector<unsigned int> randomInts(
            randomBytes.begin(), randomBytes.end());

    // Инициализация генератора случайных чисел с использованием случайных чисел
    // из файла
    std::seed_seq seed(randomInts.begin(), randomInts.end());
    std::mt19937 gen(seed);

    randomSort(lines, gen);
}

inline void
randomSort(std::vector<std::string>& lines, std::vector<std::string>& flags)
{
    auto itf = std::find_if(
            flags.begin(), flags.end(), [](const std::string& flag) {
                return flag.find("--random-source=") != std::string::npos;
            });

    if (itf != flags.end()) {
        std::string file = itf->substr(
                16); // Получаем имя файла после "--random-source="
        randomSort(
                lines,
                file); // Вызываем функцию randomSort с передачей имени файла
    }
}

// -u
inline void uniqueSort(std::vector<std::string>& lines)
{
    sortVector(lines);
    lines.erase(
            std::unique(lines.begin(), lines.end()),
            lines.end()); // Удаляем повторяющиеся строки
}

// -V
inline bool
versionCompare(const std::string& version1, const std::string& version2)
{
    std::vector<int> v1, v2;

    // Разбиваем версии на компоненты
    std::stringstream ss1(version1), ss2(version2);
    std::string token;
    while (std::getline(ss1, token, '.') && !token.empty()) {
        v1.push_back(std::stoi(token));
    }
    while (std::getline(ss2, token, '.') && !token.empty()) {
        v2.push_back(std::stoi(token));
    }

    // Сравниваем компоненты версий
    for (size_t i = 0; i < std::max(v1.size(), v2.size()); i++) {
        int component1 = (i < v1.size()) ? v1[i] : 0;
        int component2 = (i < v2.size()) ? v2[i] : 0;

        if (component1 < component2) {
            return true;
        } else if (component1 > component2) {
            return false;
        }
    }

    // Все компоненты равны
    return false;
}