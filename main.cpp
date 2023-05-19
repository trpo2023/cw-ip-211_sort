#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <string>
#include <cctype> // Для функции std::tolower
#include <locale> // Для функции std::isspace
#include <ctime> // Для функции std::mktime
#include <chrono> // Для работы с датой и временем
#include <regex> // Для работы с регулярными выражениями

bool isFileName(const std::string& str) {
    std::ifstream file(str);
    return file.good();
}

std::string removeLeadingNonPrintableChars(const std::string& str) {
    std::string cleanStr = str;

    // Удаление ведущих пробелов
    cleanStr.erase(cleanStr.begin(), std::find_if(cleanStr.begin(), cleanStr.end(), [](unsigned char c) {
        return std::isprint(c);
    }));

    return cleanStr;
}

// -b
bool ignoreLeadingSpaces(const std::string& str1, const std::string& str2) {
    size_t index1 = str1.find_first_not_of(' ');
    size_t index2 = str2.find_first_not_of(' ');
    return str1.substr(index1) < str2.substr(index2);
}

// -d
bool isNumeric(const std::string& str) {
    std::string cleanStr = str;
    cleanStr.erase(std::remove_if(cleanStr.begin(), cleanStr.end(), [](unsigned char c) { return !std::isdigit(c); }), cleanStr.end());
    return !cleanStr.empty();
}

bool numericCompare(const std::string& str1, const std::string& str2) {
    std::string cleanStr1 = removeLeadingNonPrintableChars(str1);
    std::string cleanStr2 = removeLeadingNonPrintableChars(str2);

    if (isNumeric(cleanStr1) && isNumeric(cleanStr2)) {
        return std::stoi(cleanStr1) < std::stoi(cleanStr2);
    } else if (isNumeric(cleanStr1)) {
        return true;
    } else if (isNumeric(cleanStr2)) {
        return false;
    } else {
        return cleanStr1 < cleanStr2;
    }
}

// -f
bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    size_t length = std::min(str1.length(), str2.length());
    for (size_t i = 0; i < length; ++i) {
        if (std::tolower(str1[i]) != std::tolower(str2[i])) {
            return std::tolower(str1[i]) < std::tolower(str2[i]);
        }
    }
    return str1.length() < str2.length();
}

// -g
bool ignorePunctuationAndSpaces(const std::string& str1, const std::string& str2) {
    std::locale loc;
    auto isPunctuationOrSpace = [&](char c) {
        return std::ispunct(c, loc) || std::isspace(c, loc);
    };

    auto first1 = std::find_if_not(str1.begin(), str1.end(), isPunctuationOrSpace);
    auto first2 = std::find_if_not(str2.begin(), str2.end(), isPunctuationOrSpace);
    return std::lexicographical_compare(first1, str1.end(), first2, str2.end());
}

// -h
long long extractValue(const std::string& str) {
    long long value = std::stoll(str.substr(0, str.length() - 1)); // Извлечение числового значения без суффикса
    char suffix = str.back(); // Извлечение суффикса

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

bool humanNumericCompare(const std::string& str1, const std::string& str2) {
    long long value1 = extractValue(str1);
    long long value2 = extractValue(str2);

    return value1 < value2;
}

// --help
void displayHelp() {
    std::cout << "Usage: program [OPTIONS] FILE" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -n                Sort in numeric order" << std::endl;
    std::cout << "  -b                Ignore leading spaces when sorting" << std::endl;
    std::cout << "  -d                Sort in dictionary order of digits" << std::endl;
    std::cout << "  -f                Case-insensitive sorting" << std::endl;
    std::cout << "  -g                Ignore punctuation and spaces when sorting" << std::endl;
    std::cout << "  -i                Sort printable characters only" << std::endl;
    std::cout << "  -M                Sort by month" << std::endl;
    std::cout << "  -h                Sort in human-readable format" << std::endl;
    std::cout << "  -u                Sort and remove duplicate lines" << std::endl;
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
bool isPrintable(const std::string& str1, const std::string& str2) {
    for (char c : str1) {
        if (!std::isprint(c)) {
            return false;
        }
    }
    return true;
}

// -k
void processKFlag(int columnIndex, std::vector<std::string>& lines) {
    std::cout << "Processing K flag" << std::endl;
    std::sort(lines.begin(), lines.end(), [columnIndex](const std::string& line1, const std::string& line2) {
        std::istringstream iss1(line1);
        std::istringstream iss2(line2);
        
        std::string word1, word2;
        int i = 1;
        while (i < columnIndex) {
            if (!(iss1 >> word1) || !(iss2 >> word2))
                return false; // Ошибка: Недостаточно слов в одной из строк, возвращаем false
            
            i++;
        }
        
        // Обработка случая, когда количество слов в строках равно columnIndex
        if (!(iss1 >> word1) || !(iss2 >> word2))
            return false; // Ошибка: Недостаточно слов в одной из строк, возвращаем false
        
        return word1 < word2;
    });
}

// -M
bool monthOrderCompare(const std::string& str1, const std::string& str2) {
    const std::vector<std::string> months = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    };

    std::string upperStr1 = str1;
    std::transform(upperStr1.begin(), upperStr1.end(), upperStr1.begin(), ::toupper);
    std::string upperStr2 = str2;
    std::transform(upperStr2.begin(), upperStr2.end(), upperStr2.begin(), ::toupper);

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
bool numericOrderCompare(const std::string& str1, const std::string& str2) {
    if (isNumeric(str1) && isNumeric(str2)) {
        return std::stoi(str1) < std::stoi(str2);
    } else {
        return str1 < str2;
    }
}

// -R
void randomSort(std::vector<std::string>& lines) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::shuffle(lines.begin(), lines.end(), gen);
}

// --randon-source
void randomSort(std::vector<std::string>& lines, std::mt19937& gen) {
    std::shuffle(lines.begin(), lines.end(), gen);
}

void randomSort(std::vector<std::string>& lines, std::string& randomSourceFile) {
    if (randomSourceFile.empty()) {
        std::cerr << "Error: --random-source option is missing the file path." << std::endl;
        return;
    }
    std::ifstream file(randomSourceFile, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open random source file: " << randomSourceFile << std::endl;
        return;
    }
    // Чтение случайных байтов из файла
    std::vector<char> randomBytes(std::istreambuf_iterator<char>(file), {});

    // Преобразование байтов в целочисленный тип
    std::vector<unsigned int> randomInts(randomBytes.begin(), randomBytes.end());

    // Инициализация генератора случайных чисел с использованием случайных чисел из файла
    std::seed_seq seed(randomInts.begin(), randomInts.end());
    std::mt19937 gen(seed);

    randomSort(lines, gen);
    
}

// -V
bool versionCompare(const std::string& version1, const std::string& version2) {
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


int main(int argc, char* argv[]) {
    // Переменная для хранения флагов
    std::vector<std::string> files;
    std::vector<std::string> lines;
    std::vector<std::string> flags;

    // Проверка аргументов командной строки и установка флагов
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg.substr(0, 1) == "-") {
            flags.push_back(arg);
        } else {
            // Обработка файлов
            if (isFileName(arg)) {
                files.push_back(arg);
            }
            else 
            {
                lines.push_back(arg);
            }
            
        }
    }

    // Объединение и сортировка файлов, если указаны файлы в аргументах командной строки
    if (!files.empty()) {
        // Чтение данных из файлов
        for (int i = 0; i < files.size(); ++i) {
            std::ifstream file(files[i], std::ios::in);
            if (file.is_open()) {
                std::string content;
                std::string line;
                while (std::getline(file, line)) {
                    lines.push_back(line);
                }
                    
            }
            else {
                std::cerr << "Error: file " << files[i] << " not found\n";
            }
            file.close();
        }
    }

    if (std::find(flags.begin(), flags.end(), "--help") != flags.end()) {
        displayHelp();
        return 0;
    }
    if (flags.empty()) {
        std::sort(lines.begin(), lines.end());
    }
    if (std::find(flags.begin(), flags.end(), "-n") != flags.end()) {
        std::sort(lines.begin(), lines.end(), numericOrderCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-b") != flags.end()) {
        std::sort(lines.begin(), lines.end(), ignoreLeadingSpaces);
    }
    if (std::find(flags.begin(), flags.end(), "-d") != flags.end()) {
        std::sort(lines.begin(), lines.end(), numericCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-f") != flags.end()) {
        std::sort(lines.begin(), lines.end(), caseInsensitiveCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-g") != flags.end()) {
        std::sort(lines.begin(), lines.end(), ignorePunctuationAndSpaces);
    }
    if (std::find(flags.begin(), flags.end(), "-i") != flags.end()) {
        std::sort(lines.begin(), lines.end(), isPrintable);
    }
    if (std::find(flags.begin(), flags.end(), "-M") != flags.end()) {
        std::sort(lines.begin(), lines.end(), monthOrderCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-h") != flags.end()) {
        std::sort(lines.begin(), lines.end(), humanNumericCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-u") != flags.end()) {
        std::sort(lines.begin(), lines.end()); // Сортируем строки
        lines.erase(std::unique(lines.begin(), lines.end()), lines.end()); // Удаляем повторяющиеся строки
    }
    if (std::find(flags.begin(), flags.end(), "-R") != flags.end()) {
        randomSort(lines);
    }
    auto itf = std::find_if(flags.begin(), flags.end(), [](const std::string& flag) {
        return flag.find("--random-source=") != std::string::npos;
    });

    if (itf != flags.end()) {
        std::string file = itf->substr(16); // Получаем имя файла после "--random-source="
        randomSort(lines, file); // Вызываем функцию randomSort с передачей имени файла
    }
    if (std::find(flags.begin(), flags.end(), "-V") != flags.end()) {
        std::sort(lines.begin(), lines.end(), versionCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-c") != flags.end()) {
        if (std::is_sorted(lines.begin(), lines.end())) {
            std::cout << "Sorted\n";
        }
        else {
            std::cout << "Not sorted\n";
        }
    }
    auto it = std::find_if(flags.begin(), flags.end(), [](const std::string& flag) {
        return flag.size() >= 2 && flag.substr(0, 2) == "-k";
    });

    if (it != flags.end()) {
        std::string digit = it->substr(2);

        if (!digit.empty()) {
            int columnIndex = std::stoi(digit);
            processKFlag(columnIndex, lines);
        }
    }
    if (std::find(flags.begin(), flags.end(), "-r") != flags.end()) {
        if (flags.size() == 1) {
            std::sort(lines.begin(), lines.end());
        }
        std::reverse(lines.begin(), lines.end());
    }

    // Вывод отсортированных данных в стандартный вывод
    for (const auto& sortedLine : lines) {
        std::cout << sortedLine << '\n';
    }
    return 0;
}