#include <vector>
#include <string>


/**
 * @brief Prints the main menu.
 * This function prints the main menu of the application.
 */
auto printMenu() -> void;


/**
 * @brief Returns the available source files.
 * This function returns available, prepared source files.
 * @return A vector of source file paths.
 */
auto getSources() -> std::vector<std::string>;


/**
 * @brief Allows you to select the source file.
 * This function allows you to select a source file by selecting it from a list or by specifying its absolute path.
 * @return Returns the path of the selected file.
 */
auto selectFile() -> std::string;


/**
 *
 * @param path Path to file.
 * @param password Password provided by user.
 * @return true - if the password is correct, false - if it is not.
 */
auto checkFilePassword(const std::string& path, const std::string& password) -> bool;


/**
 * @brief Allows you to select the command.
 * This function prints a list of available commands, allows you to select one and returns int representing
 * the selected one.
 * @return int representing selected command.
 */
auto selectCommand() -> int;

/**
 * @brief Allows you to answer the program's questions.
 * This function allows you to answer simple questions asked by the program and returns a bool that represents
 * the user's answer.
 * @return Represents user response. Returns true if the answer was "yes" and false if the answer was "no."
 */
auto getShortAnswer() -> bool;


auto getLongAnswer() -> int;