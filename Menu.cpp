#include "Menu.h"
#include "Code.h"
#include <fmt/core.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

auto printMenu() -> void {
    using fmt::print;
    print(" ====================== Password Manager ====================== \n");
    print(" * These are the available source files. Select one by typing its number, or type the path to another text file.\n");
    auto sources = getSources();
    for (auto i = 0; i < sources.size(); i++)
        print("{}. {}\n", i + 1, sources[i]);
}

auto getSources() -> std::vector<std::string>{
    auto paths = std::vector<std::string>();
    auto dirIterator = std::filesystem::directory_iterator(
            "/Users/kubasajnog/CLionProjects/Passwords_Manager_4.0/Sources");
    for (const auto &nextPath: dirIterator)
        paths.push_back(nextPath.path());
    return paths;
}

auto selectFile() -> std::string {
    auto sources = getSources();
    auto userInput = std::string();
    auto isCorrectFile = [&userInput]() {
        auto extension = std::filesystem::path(userInput).extension().string();
        return std::filesystem::exists(userInput) && extension == ".txt";
    };
    while (true) {
        std::getline(std::cin, userInput);
        try {
            auto fileNumber = std::stoi(userInput);
            if (fileNumber <= 0 || fileNumber > sources.size())
                fmt::print("\n * Wrong number, try again\n");
            else
                return sources[fileNumber - 1];
        } catch (const std::invalid_argument &e) {
            if (isCorrectFile())
                return userInput;
            else
                fmt::print("\n * Incorrect file selection, try again.\n");
        }
    }
}

auto checkFilePassword(const std::string &path, const std::string &password) -> bool {
    auto fstream = std::fstream(path);
    auto firstLine = std::string();
    getline(fstream, firstLine);
    firstLine = decodeLine(firstLine, password);
    return firstLine == "UNENCRYPTED";
}

auto selectCommand() -> int{
    while (true) {
        auto commandNumber = std::map<std::string, int>{
                {"SEARCH_PASSWORDS", 1},
                {"SORT_PASSWORDS",   2},
                {"ADD_PASSWORD",     3},
                {"EDIT_PASSWORD",    4},
                {"DELETE_PASSWORD",  5},
                {"ADD_CATEGORY",     6},
                {"DELETE_CATEGORY",  7},
                {"LIST_CATEGORIES",  8},
                {"LIST_PASSWORDS",   9},
                {"SEE_TIMESTAMP",   10},
                {"EXIT",             11}
        };
        fmt::print("\n * What would you like to do? Select command: (You can enter '~' in any place to return there.)\n"
                   "\t 1. SEARCH_PASSWORDS\n"
                   "\t 2. SORT_PASSWORDS\n"
                   "\t 3. ADD_PASSWORD,\n"
                   "\t 4. EDIT_PASSWORD,\n"
                   "\t 5. DELETE_PASSWORD,\n"
                   "\t 6. ADD_CATEGORY,\n"
                   "\t 7. DELETE_CATEGORY,\n"
                   "\t 8. LIST_CATEGORIES\n"
                   "\t 9. LIST_PASSWORDS\n"
                   "\t 10. SEE_TIMESTAMP\n"
                   "\t 11. EXIT\n");
        auto userInput = std::string();
        std::getline(std::cin, userInput);
        try {
            auto num = std::stoi(userInput);
            if(num > 0 && num <= commandNumber.size()) return num;
            else fmt::print("\n * Incorrect command selection, try again.\n");
        } catch (const std::invalid_argument &e){
            std::ranges::transform(userInput, userInput.begin(), ::toupper);
            if (commandNumber[userInput] == 0)
                fmt::print("\n * Incorrect command selection, try again.\n");
            else return commandNumber[userInput];
        }
    }
}

auto getShortAnswer() -> bool{
    auto answer = std::string();
    while (true){
        std::getline(std::cin, answer);
        std::ranges::transform(answer, answer.begin(),::toupper);
        switch (answer[0]) {
            case 'Y':
                return true;
            case 'N':
                return false;
            default:
                fmt::print("\n * Wrong answer, try again. Enter yes or no.\n");
        }
    }
}


auto getLongAnswer() -> int {
    auto answer = std::string();
    while (true) {
        std::getline(std::cin, answer);
        if (answer == "~") throw std::invalid_argument("~");
        try {
            auto num = std::stoi(answer);
            if (num > 0 && num <= 5) return num;
            else fmt::print("\n * Incorrect command selection, try again.\n");
        } catch (const std::invalid_argument &e) {
            std::ranges::transform(answer, answer.begin(), ::toupper);
            if (answer == "NAME") return 1;
            else if (answer == "CATEGORY") return 2;
            else if (answer == "SERVICE") return 3;
            else if (answer == "LOGIN") return 4;
            else if (answer == "PASSWORD") return 5;
            else fmt::print("* Wrong choice. Try again.\n");
        }

    }
}