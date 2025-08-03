#ifndef AUTH_HPP
#define AUTH_HPP

#include <string>

class Auth {
public:
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void logoutUser();

private:
    std::string currentUser;
};

#endif // AUTH_HPP