#include <vector>
#include <sstream>
#include "Password.h"

Password::Password(const std::string &name, const std::string &password, const std::string &category,
                   const std::string &service, const std::string &login) {
    this->name = name;
    this->password = password;
    this->category = category;
    if(service.empty()) this->service = "Unassigned";
    else this->service = service;
    if(login.empty()) this->login = "Unassigned";
    else this->login = login;
}

Password::Password(const std::string &line) {
    auto iss = std::istringstream(line);
    auto words = std::vector<std::string>();
    auto word = std::string();
    while (std::getline(iss, word, ','))
        words.push_back(word);
    name = words[0];
    password = words[1];
    category = words[2];
    if(words[3] == " ") service = "Unassigned";
    else service = words[3];
    if(words[4] == " ")  login = "Unassigned";
    else login = words[4];
}

const std::string &Password::getCategory() const {
    return category;
}

const std::string &Password::getName() const {
    return name;
}

const std::string &Password::getPassword() const {
    return password;
}

const std::string &Password::getService() const {
    return service;
}

const std::string &Password::getLogin() const {
    return login;
}




void Password::setName(const std::string &name) {
    Password::name = name;
}

void Password::setPassword(const std::string &password) {
    Password::password = password;
}

void Password::setCategory(const std::string &category) {
    Password::category = category;
}

void Password::setService(const std::string &service) {
    Password::service = service;
}

void Password::setLogin(const std::string &login) {
    Password::login = login;
}

auto Password::toString() const -> std::string {
    return  "NAME: " + this->name + " PASSWORD: " + this->password + " CATEGORY: " + this->category + " SERVICE: " + this->service
            + " LOGIN: " + this->login;
}


