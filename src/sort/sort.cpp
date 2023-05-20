#include "../libsort/sort.h"

int main(int argc, char* argv[])
{
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
            } else {
                lines.push_back(arg);
            }
        }
    }

    // Объединение и сортировка файлов, если указаны файлы в аргументах
    // командной строки
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

            } else {
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
        sortVector(lines);
    }
    if (std::find(flags.begin(), flags.end(), "-n") != flags.end()) {
        sortVector(lines, numericOrderCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-b") != flags.end()) {
        sortVector(lines, ignoreLeadingSpaces);
    }
    if (std::find(flags.begin(), flags.end(), "-d") != flags.end()) {
        sortVector(lines, dictionaryOrderCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-f") != flags.end()) {
        sortVector(lines, caseInsensitiveCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-g") != flags.end()) {
        sortVector(lines, ignorePunctuationAndSpaces);
    }
    if (std::find(flags.begin(), flags.end(), "-i") != flags.end()) {
        sortVector(lines, isPrintable);
    }
    if (std::find(flags.begin(), flags.end(), "-M") != flags.end()) {
        sortVector(lines, monthOrderCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-h") != flags.end()) {
        sortVector(lines, humanNumericCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-u") != flags.end()) {
        uniqueSort(lines);
    }
    if (std::find(flags.begin(), flags.end(), "-R") != flags.end()) {
        randomSort(lines);
    }
    randomSort(lines, flags);
    if (std::find(flags.begin(), flags.end(), "-V") != flags.end()) {
        sortVector(lines, versionCompare);
    }
    if (std::find(flags.begin(), flags.end(), "-c") != flags.end()) {
        if (checkSorted(lines)) {
            std::cout << "Sorted\n";
        } else {
            std::cout << "Not sorted\n";
        }
    }
    kSortVector(lines, flags);
    if (std::find(flags.begin(), flags.end(), "-r") != flags.end()) {
        reverseSort(lines);
    }

    // Вывод отсортированных данных в стандартный вывод
    for (const auto& sortedLine : lines) {
        std::cout << sortedLine << '\n';
    }
    return 0;
}