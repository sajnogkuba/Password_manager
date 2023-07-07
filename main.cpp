#include "Menu.h"
#include "Executor.h"
#include "Code.h"
#include <iostream>
#include <filesystem>
#include <fmt/core.h>

int main() {

    printMenu();
    auto selectedFile = selectFile();
    auto filePass = std::string();

    if(std::filesystem::file_size(selectedFile)== 0) {
        fmt::print("\n * This file is empty. Enter the password with which it will be secured.\n");
        std::getline(std::cin, filePass);
        prepareFile(selectedFile, filePass);
    }
    else{
        fmt::print("\n * Enter the password for this file\n");
        deleteTimestamp(selectedFile);
        std::getline(std::cin, filePass);
        if(!checkFilePassword(selectedFile, filePass)) {
            fmt::print("\n * Incorrect password, file cannot be decrypted!\n");
            addTimestapm(selectedFile);
            exit(0);
        }
    }
    auto exit = false;
    auto executor = Executor(selectedFile, filePass);
    addTimestapm(selectedFile);
    while (!exit) {
        auto selectedCommand = selectCommand();
        if(selectedCommand == 11) exit = true;
        executor.execute(selectedCommand);
    }
    executor.save();
    addTimestapm(selectedFile);
}
