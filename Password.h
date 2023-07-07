
#include <string>

class Password{
private:
    std::string name;
    std::string password;
    std::string category;
    std::string service;
    std::string login;
public:
    Password(const std::string& name, const std::string& password, const std::string& category,
             const std::string& service, const std::string& login);


    /**
     * @brief This constructor creates a new object based on a line from the source file.
     * @param line A line of text based on which a new object will be created.
     */
    Password(const std::string& line);

    const std::string &getCategory() const;

    const std::string &getName() const;

    const std::string &getPassword() const;

    const std::string &getService() const;

    const std::string &getLogin() const;

    void setName(const std::string &name);

    void setPassword(const std::string &password);

    void setCategory(const std::string &category);

    void setService(const std::string &service);

    void setLogin(const std::string &login);

    auto toString() const-> std::string;

    bool operator==(const Password& other) const {
        return name == other.name;
    }


};

