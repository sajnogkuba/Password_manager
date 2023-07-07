
#include "Password.h"
#include <string>
#include <vector>
#include <set>

class Executor{
private:
    std::string path;
    std::string filePass;
    std::set<std::string> categories;
    std::vector<Password> passwords;

    /**
     * @brief Adds categories.
     * This function adds password categories to the selected file.
     */
    auto addCategory()-> void;

    /**
     * @brief Adds password.
     * This function adds a password to the selected file. Allows you to give the password a name,
     * select a category, etc.
     */
    auto addPassword() -> void;

    /**
     * @brief Lists available categories.
     * This function prints a list of available categories in the selected file.
     */
    auto listCategories() const -> void;

    /**
     * @brief Deletes categories.
     * This function deletes password categories from the selected file and all passwords belonging to this category.
     */
    auto deleteCategory() -> void;


    /**
     * @brief Generate vector of passwords from source file.
     * @return Vectors of passwords from source file.
     */
    auto getPasswords() const -> std::vector<Password>;

    /**
     * @brief Returns a vector of available categories.
     * This function checks what categories are available in the selected file and returns them in a vector.
     * @return Vector of available categories.
     */
    auto getCategories() const->  std::set<std::string>;

    /**
     * @brief Allows you to specify a new password name.
     * This function allows you to enter a new name for the password, checks if it is correct and returns it if so.
     * @return new password name.
     */
    auto passName() -> std::string;


    /**
     * @brief Lists available categories.
     * This function prints a list of passwords in the selected file.
     */
    auto listPasswords() const-> void;

    /**
     * @brief Deletes passwords from the selected file.
     * This function finds the passwords that the user wants to delete, and then removes them from the source file.
     */
    auto deletePasswords() -> void;

    /**
     * @brief Search passwords.
     * This function searches for passwords and prints a list of matches.
     */
    auto searchPassword() -> void;

    /**
     * @brief Allows you to edit existing password.
     */
    auto editPassword() -> void;

    /**
     * @brief Prints a list of sorted passwords.
     * This function sorts all passwords from a file and prints a list of them. You can select more than one parameter
     * for sorting.
     */
    auto sortPasswords() -> void;

    /**
     * @brief Prints a time stamp of last modification of selected file.
     */
    auto seeTimestamp() -> void;


    auto matchingVector() -> std::vector<Password>;
    auto passCategory() -> std::string;
    auto passText() -> std::string;
public:
    Executor(const std::string &path, const std::string &filePass);

    /**
     * @brief Executes the selected command.
     * @param commandNumber The number of the command to be executed.
     */
    auto execute(int commandNumber) -> void;

    /**
     * @brief Save the file.
     * This function is adding information from vector passwords to file which path is represented in field path.
     */
    auto save() -> void;
};


auto containsComma(const std::string& toCheck) -> bool;
auto safetyPass(const std::string &password) -> int;
auto passGenerator() -> std::string;
auto passLogin() -> std::string;
auto passService() -> std::string;