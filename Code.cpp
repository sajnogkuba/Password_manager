#include <chrono>
#include <vector>
#include <fstream>
#include "Code.h"3



auto codeLine(const std::string &line, const std::string &password) -> std::string {

    auto codedLine = std::string();
    for (auto i = 0; i < line.size(); i++) {
        auto codedChar = char(((line[i] ^ password[i % password.size()]) + password.size() + 31));
        codedLine += codedChar;
    }
    return codedLine;
}


auto decodeLine(const std::string &line, const std::string &password) -> std::string {
    auto decodedLine = std::string();

    for (auto i = 0; i < line.size(); i++) {
        auto decodedChar = char(((line[i] - password.size() -31) ^ password[i % password.size()])) ;
        decodedLine += decodedChar;
    }
    return decodedLine;
}

auto addTimestapm(const std::string &file) -> void {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm *timeInfo = std::localtime(&currentTime);

    int hour = timeInfo->tm_hour;
    int minute = timeInfo->tm_min;
    int second = timeInfo->tm_sec;

    hour %= 100;
    minute %= 100;
    second %= 100;

    auto sHour = std::string();
    if (hour < 10) {
        sHour = std::string("0");
        sHour += std::to_string(hour);
    } else sHour = std::to_string(hour);

    auto sMinute = std::string();
    if (minute < 10) {
        sMinute = std::string("0");
        sMinute += std::to_string(minute);
    } else sMinute = std::to_string(minute);

    auto sSecond = std::string();
    if (second < 10) {
        sSecond = std::string("0");
        sSecond += std::to_string(second);
    } else sSecond = std::to_string(second);

    auto line = std::string();
    std::vector<std::string> lines = std::vector<std::string>();
    auto ifstream = std::ifstream(file);
    while (std::getline(ifstream, line)) {
        lines.push_back(line);
    }
    ifstream.close();

    if (lines[9].empty()) lines[9] = sHour;
    else lines[9] = sHour + lines[9];
    if (lines[19].empty()) lines[19] = sMinute;
    else lines[19] = sMinute + lines[19];
    if (lines[29].empty()) lines[29] = sSecond;
    else lines[29] = sSecond + lines[29];

    auto ofstram = std::ofstream(file);
    for (const auto &l: lines) {
        ofstram << l << '\n';
    }
    ofstram.close();
}

auto deleteTimestamp(const std::string &file) -> void {
    auto line = std::string();
    std::vector<std::string> lines = std::vector<std::string>();
    auto ifstream = std::ifstream(file);
    while (std::getline(ifstream, line))
        lines.push_back(line);
    lines[9] = lines[9].substr(2);
    lines[19] = lines[19].substr(2);
    lines[29] = lines[29].substr(2);

    auto ofstram = std::ofstream(file);
    for (const auto &l: lines) {
        ofstram << l << '\n';
    }
    ofstram.close();
}

auto prepareFile(const std::string &path, const std::string &filePass) -> void {
    auto fstream = std::fstream(path);
    fstream << codeLine("UNENCRYPTED", filePass) << std::endl;
    for (auto i = 0; i < 29; i++) fstream << std::endl;
}

