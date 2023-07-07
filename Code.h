#include <string>

/**
 * @brief Code the line, using password.
 * This function is coding line with password.
 * @param line Line to be coded.
 * @param password Password to code the line.
 * @return Coded line.
 */
auto codeLine(const std::string& line, const std::string& password) -> std::string;

/**
 * @brief Decode the line, using password.
 * This function is decoding line with password.
 * @param line Line to be decoded.
 * @param password Password to decode the line.
 * @return Decoded line.
 */
auto decodeLine(const std::string& line, const std::string& password) -> std::string;

/**
 *
 * @param path Path to file.
 * @param password Password provided by user.
 * @return true - if the password is correct, false - if it is not.
 */
auto checkFilePassword(const std::string& path, const std::string& password) -> bool;

/**
 * @brief This function adds a timestamp to a file.
 * @param file path to file to add timestamp.
 */
auto addTimestapm(const std::string& file) -> void;

/**
 * @brief This function delete a timestamp from a file.
 * @param file path to file to delete timestamp.
 */
auto deleteTimestamp(const std::string& file) -> void;

/**
 * @brief Adds to the file the lines needed for the program to work properly.
 * @param path Path to file to prepare.
 * @param mode Operation mode. Another for an empty file, another for a non-empty file.
 */
auto prepareFile(const std::string& path, const std::string &filePass) -> void;