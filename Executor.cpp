#include "Executor.h"
#include "Code.h"
#include "Menu.h"
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <set>

Executor::Executor(const std::string &path, const std::string &filePass) : path(path), filePass(filePass) {
    this->path = path;
    this->filePass = filePass;
    this->passwords = getPasswords();
    this->categories = getCategories();

}

auto Executor::execute(int commandNumber) -> void {
    switch (commandNumber) {
        case 1:
            if (passwords.empty()) {
                fmt::print("\n * First, create at least one password.\n");
                return;
            }
            searchPassword();
            break;
        case 2:
            sortPasswords();
            break;
        case 3:
            if (categories.empty()) {
                fmt::print("\n * First, create at least one category.\n");
                return;
            }
            addPassword();
            break;
        case 4:
            if (passwords.empty()) {
                fmt::print("\n * First, create at least one password.\n");
                return;
            }
            editPassword();
            break;
        case 5:
            if (passwords.empty()) {
                fmt::print("\n * First, create at least one password.\n");
                return;
            }
            deletePasswords();
            break;
        case 6:
            addCategory();
            break;
        case 7:
            deleteCategory();
            break;
        case 8:
            if (categories.empty()) fmt::print("\n * There are no categories in this file.\n");
            else listCategories();
            break;
        case 9:
            if (passwords.empty()) {
                fmt::print("\n * First, create at least one password.\n");
                return;
            }
            listPasswords();
            break;
        case 10:
            seeTimestamp();
            break;
        default: {
        }
    }
}


auto Executor::addCategory() -> void {
    while (true) {
        fmt::print("\n * Enter the category to add.\n");
        auto userInput = std::string();
        std::getline(std::cin, userInput);
        if (userInput == "~") break;
        if (containsComma(userInput)) continue;
        if (std::ranges::find(categories, userInput) != categories.end()) {
            fmt::print("\n * This category already exists, try again\n");
            continue;
        }
        categories.insert(userInput);
        fmt::print("\n * Want to add another category?\n");
        if (!getShortAnswer()) break;
    }
}

auto Executor::listCategories() const -> void {
    fmt::print("\n * Categories in this file:\n");
    for (auto category: categories)
        fmt::print("\t- {}\n", category);
}

auto Executor::addPassword() -> void {
    while (true) {
        auto name = passName();
        if (name == "~") break;
        auto text = std::string();
        auto service = std::string();
        auto login = std::string();
        fmt::print("\n * Want to generate a password automatically?\n");
        if (getShortAnswer()) {
            text = passGenerator();
            if (text == "~") break;
        } else {
            text = passText();
            if (text == "~") break;
        }
        auto category = passCategory();
        if (category == "~") break;
        fmt::print("\n * Want to assign a login?\n");
        if (getShortAnswer()) {
            login = passLogin();
            if (login == "~") break;
        }
        fmt::print("\n * Want to assign a service?\n");
        if (getShortAnswer()) {
            service = passService();
            if (service == "~") break;
        }
        passwords.emplace_back(name, text, category, service, login);
        fmt::print("\n * Want to add another password?\n");
        if (!getShortAnswer()) break;
    }

}

auto Executor::deleteCategory() -> void {
    while (true) {
        fmt::print("\n * Enter the category to delete.\n");
        auto toDelete = passCategory();
        if (toDelete == "~") break;
        auto passInIt = 0;
        for (const auto &pass: passwords) if (pass.getCategory() == toDelete) passInIt++;
        fmt::print(" * If you delete this category \"{}\" you will lose {} passwords."
                   " Are you sure you want to delete it?\n", toDelete, passInIt);
        if (getShortAnswer()) {
            categories.erase(std::ranges::find(categories, toDelete));
            std::erase_if(passwords,
                          [&toDelete](const Password &password) { return password.getCategory() == toDelete; });
        }
        fmt::print(" * Do you want to delete another category?.\n");
        if (!getShortAnswer()) break;
    }
}

auto Executor::deletePasswords() -> void {
    while (true) {
        fmt::print("\n * Search password to delete\n");
        try {
            auto matchingPasswords = matchingVector();
            if (matchingPasswords.empty()) fmt::print(" * No passwords found for deletion.\n");
            else {
                fmt::print(" * {} passwords found for deletion:\n", matchingPasswords.size());
                for (const auto &pass: matchingPasswords)
                    fmt::print("\t 1 {}\n", pass.toString());
                fmt::print(" * Are you sure you want to remove them?\n");
                if (getShortAnswer())
                    for (const auto &pass: matchingPasswords)
                        std::erase_if(passwords, [pass](auto p) { return p == pass; });
            }
        }
        catch (const std::invalid_argument &e) { break; }
        fmt::print(" * Would you like to remove something else?\n");
        if (!getShortAnswer()) break;
    }
}

auto Executor::searchPassword() -> void {
    while (true) {
        try {
            auto matchingPasswords = matchingVector();
            if (matchingPasswords.empty()) fmt::print(" * No matching passwords found. :< \n");
            else {
                fmt::print(" * {} passwords match ;> : \n", matchingPasswords.size());
                for (const auto &pass: matchingPasswords)
                    fmt::print("\t - {}\n", pass.toString());
            }
        } catch (const std::invalid_argument &e) { break; }
        fmt::print(" * Want to search again?\n");
        if (!getShortAnswer()) break;
    }
}

auto Executor::listPasswords() const -> void {
    fmt::print("\n * Passwords in this file:\n");
    for (auto i = 0; i < passwords.size(); i++)
        fmt::print("\t{}. {}\n", i + 1, passwords[i].toString());
}

auto Executor::editPassword() -> void {
    auto exit = false;
    while (!exit) {
        listPasswords();
        fmt::print("\n * Select password to edit by number or name\n");
        auto searchName = std::string();
        std::getline(std::cin, searchName);
        if (searchName == "~") break;
        Password *toEdit;
        try {
            auto number = std::stoi(searchName);
            if (number <= 0 || number > passwords.size()) {
                fmt::print(" * Incorrect number. Try again.");
                continue;
            }
            toEdit = &passwords[number - 1];
        } catch (const std::invalid_argument &) {
            auto it = std::ranges::find(passwords, searchName, &Password::getName);
            if (it == passwords.end()) {
                fmt::print(" * Password with the specified name not found, try again.\n");
                continue;
            }
            toEdit = &*it;
        }
        fmt::print(" * What would you like to edit?\n"
                   "\t1. NAME\n"
                   "\t2. CATEGORY\n"
                   "\t3. SERVICE\n"
                   "\t4. LOGIN\n"
                   "\t5. PASSWORD\n");
        auto answer = int();
        try { answer = getLongAnswer(); }
        catch (const std::invalid_argument &) { break; }
        auto input = std::string();
        switch (answer) {
            case 1:
                input = passName();
                if (input == "~") exit = true;
                toEdit->setName(input);
                break;
            case 2:
                input = passCategory();
                if (input == "~") exit = true;
                toEdit->setCategory(input);
                break;
            case 3:
                input = passService();
                if (input == "~") exit = true;
                toEdit->setService(input);
                break;
            case 4:
                input = passLogin();
                if (input == "~") exit = true;
                toEdit->setLogin(input);
                break;
            case 5:
                fmt::print("\n * Want to generate a password automatically?\n");
                if (getShortAnswer()) {
                    input = passGenerator();
                    if (input == "~") exit = true;
                } else {
                    input = passText();
                    if (input == "~") exit = true;
                }
                toEdit->setPassword(input);
                break;
            default: {
            }
        }
        fmt::print("\n * Want to edit something else?\n");
        if(!getShortAnswer()) exit = true;
    }

}

auto Executor::sortPasswords() -> void {
    auto sortingOptions = std::vector<int>();
    while (true) {
        fmt::print(" * Select a sorting option:\n"
                   "\t1. NAME\n"
                   "\t2. CATEGORY\n"
                   "\t3. SERVICE\n"
                   "\t4. LOGIN\n"
                   "\t5. PASSWORD\n");


        auto answer = int();
        try { answer = getLongAnswer(); }
        catch (const std::invalid_argument &) { return; }
        sortingOptions.push_back(answer);
        fmt::print(" * Want to add a next sorting parameter?\n");
        if (!getShortAnswer()) break;
    }
    for (auto i = sortingOptions.size() ; i > 0; i--) {
        switch (sortingOptions[i-1]) {
            case 1:
                std::ranges::stable_sort(passwords, {}, [](const Password& password)
                {return password.getName();});
                break;
            case 2:
                std::ranges::stable_sort(passwords, {}, [](const Password& password)
                {return password.getCategory();});
                break;
            case 3:
                std::ranges::stable_sort(passwords, {}, [](const Password& password)
                {return password.getService();});
                break;
            case 4:
                std::ranges::stable_sort(passwords, {}, [](const Password& password)
                {return password.getLogin();});
                break;
            case 5:
                std::ranges::stable_sort(passwords, {}, [](const Password& password)
                {return password.getPassword();});
                break;
        }
    }
    listPasswords();
}


auto Executor::getPasswords() const -> std::vector<Password> {
    auto nextLine = std::string();
    auto ifstream = std::ifstream(this->path);
    auto pass = std::vector<Password>();
    std::getline(ifstream, nextLine);
    while (std::getline(ifstream, nextLine)) {
        if (!nextLine.empty())
            pass.emplace_back(decodeLine(nextLine, filePass));
    }
    return pass;
}

auto Executor::getCategories() const -> std::set<std::string> {
    auto categoriesIn = std::set<std::string>();
    for (const auto &pass: passwords)
        categoriesIn.insert(pass.getCategory());
    return categoriesIn;
}

auto Executor::passName() -> std::string {
    auto input = std::string();
    while (true) {
        fmt::print(" * Enter password name.\n");
        std::getline(std::cin, input);
        if (input == "~") return input;
        if (containsComma(input)) continue;
        if (std::ranges::find(passwords, input, &Password::getName) == passwords.end())
            return input;
        else
            fmt::print(" * A password with this name already exists in this file. Try again.\n");
    }
}

auto Executor::passText() -> std::string {
    auto input = std::string();
    while (true) {
        fmt::print(" * Enter password.\n");
        std::getline(std::cin, input);
        if (input == "~") return input;
        if (std::ranges::find(passwords, input, &Password::getPassword) != passwords.end()) {
            fmt::print("\n * This password has been used before. Are you sure you want to use it again?\n");
            if (!getShortAnswer()) continue;
        }
        if (containsComma(input)) continue;
        auto safety = safetyPass(input);
        fmt::print(" * Safety evaluation: {}/5\n", safety);
        fmt::print(" * Do you want to continue?\n");
        if (getShortAnswer()) return input;
    }
}

auto Executor::passCategory() -> std::string {
    while (true) {
        listCategories();
        fmt::print(" * Select one of the above categories for your new password. Enter '~' to back to menu.\n");
        auto input = std::string();
        std::getline(std::cin, input);
        if (input == "~") return input;
        if (std::ranges::find(categories, input) == categories.end()) {
            fmt::print(" * There is no such category in this file. Do you want to add it?\n");
            if (getShortAnswer()) categories.insert(input);
            else continue;
        }
        return input;
    }
}

auto Executor::matchingVector() -> std::vector<Password> {
    fmt::print("* You can search by:\n"
               "\t 1. name\n"
               "\t 2. password\n"
               "\t 3. category\n"
               "\t 4. service\n"
               "\t 5. login\n"
               "* Choose one option and enter it.\n");

    auto answ = getLongAnswer();
    auto toFind = std::string();
    switch (answ) {
        case 1:
            fmt::print(" * Enter a name to find.\n");
            break;
        case 2:
            fmt::print(" * Enter a password to find.\n");
            break;
        case 3:
            fmt::print(" * Enter a category to find.\n");
            break;
        case 4:
            fmt::print(" * Enter a service to find.\n");

            break;
        case 5:
            fmt::print(" * Enter a login to find.\n");
            break;
        default: {
        }
    }
    std::getline(std::cin, toFind);
    auto matchingPasswords = std::vector<Password>();
    switch (answ) {
        case 1:
            for (const auto &pass: passwords)
                if (pass.getName().find(toFind) != std::string::npos)
                    matchingPasswords.push_back(pass);
            break;
        case 2:
            for (const auto &pass: passwords)
                if (pass.getPassword().find(toFind) != std::string::npos) matchingPasswords.push_back(pass);
            break;
        case 3:
            for (const auto &pass: passwords)
                if (pass.getCategory().find(toFind) != std::string::npos) matchingPasswords.push_back(pass);
            break;
        case 4:
            for (const auto &pass: passwords)
                if (pass.getService().find(toFind) != std::string::npos) matchingPasswords.push_back(pass);
            break;
        case 5:
            for (const auto &pass: passwords)
                if (pass.getLogin().find(toFind) != std::string::npos) matchingPasswords.push_back(pass);
            break;
        default: {
        }
    }
    return matchingPasswords;
}

auto Executor::save() -> void {
    std::ofstream file(path, std::ofstream::trunc);
    auto fstream = std::fstream(path);
    fstream << codeLine("UNENCRYPTED", filePass) << '\n';
    for(const auto &pass : passwords){
        auto line =  pass.getName() + ',' + pass.getPassword() + ',' + pass.getCategory() + ',' +
                            pass.getService() + ',' + pass.getLogin();
        fstream << codeLine(line, filePass) << std::endl;
    }
    if(passwords.size() + 1 < 30)
        for(auto i = 0; i < 30 - passwords.size() - 1; i++)
            fstream << std::endl;

}

auto Executor::seeTimestamp() -> void {
    auto fstream = std::ifstream(path);
    auto count = 0;
    auto line = std::string();

    auto timeStamp = std::string();
    while (getline(fstream, line)){
        count++;
        if(count == 10 || count == 20) timeStamp += line.substr(0, 2) + ':';
        else if(count == 30){
            timeStamp += line.substr(0, 2);
            break;
        }
    }

    fmt::print(" * Last modification: {}\n", timeStamp);
}


auto containsComma(const std::string &toCheck) -> bool {
    if (std::ranges::find(toCheck, ',') != toCheck.end()) {
        fmt::print("\n * It can't contains comma. Try again.\n");
        return true;
    }
    return false;
}

auto safetyPass(const std::string &password) -> int {
    auto safety = 0;
    if (password.length() >= 8) safety += 1;
    if (std::ranges::any_of(password, [](char c) { return std::islower(c); })) safety += 1;
    if (std::ranges::any_of(password, [](char c) { return std::isdigit(c); })) safety += 1;
    if (std::ranges::any_of(password, [](char c) { return std::isupper(c); })) safety += 1;
    if (std::ranges::any_of(password, [](char c) { return std::ispunct(c); })) safety += 1;
    return safety;
}

auto passGenerator() -> std::string {
    auto numbers = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto capitalLetters = std::vector<char>();
    for (auto c = 'A'; c <= 'Z'; c++) capitalLetters.push_back(c);
    auto letters = std::vector<char>();
    for (auto c = 'a'; c <= 'z'; c++) letters.push_back(c);

    auto specials = std::vector<char>{'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '+', '=', '[', ']',
                                      '{', '}', ';', '<', '>', '.', '/', '?', '\\', '|', '~'};
    auto length = int();
    while (true) {
        fmt::print(" * How many characters should it have?\n");
        auto input = std::string();
        getline(std::cin, input);
        if (input == "~") return input;
        try {
            length = std::stoi(input);
        } catch (std::invalid_argument &e) {
            fmt::print(" * Something went wrong, try again.\n");
            continue;
        }
        break;
    }
    fmt::print(" * Should it contain capital letters?\n");
    auto capital = getShortAnswer();
    fmt::print(" * Should it contain special characters\n");
    auto special = getShortAnswer();
    auto generatedPass = std::string();
    auto random = std::mt19937(std::random_device{}());
    auto distribution = std::uniform_int_distribution<int>(0, 99);
    while (generatedPass.size() != length) {
        auto randomNum = distribution(random) % 4;
        if (randomNum == 0)
            generatedPass += letters[(int) distribution(random) % letters.size()];
        else if (randomNum == 1)
            generatedPass += std::to_string(numbers[(int) distribution(random) % numbers.size()]);
        else if (randomNum == 2 && special)
            generatedPass += specials[(int) distribution(random) % letters.size() % specials.size()];
        else if (randomNum == 3 && capital)
            generatedPass += capitalLetters[(int) distribution(random) % letters.size() % capitalLetters.size()];
    }
    return generatedPass;
}

auto passLogin() -> std::string {
    auto input = std::string();
    while (true) {
        fmt::print(" * Enter a login:\n");
        getline(std::cin, input);
        if (containsComma(input)) continue;
        else break;
    }
    return input;
}

auto passService() -> std::string {
    auto input = std::string();
    while (true) {
        fmt::print(" * Enter a service:\n");
        getline(std::cin, input);
        if (containsComma(input)) continue;
        else break;
    }
    return input;
}
