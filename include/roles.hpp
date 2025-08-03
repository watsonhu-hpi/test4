#ifndef ROLES_HPP
#define ROLES_HPP

#include <string>
#include <map>

class Roles {
public:
    void assignRole(const std::string& username, const std::string& role);
    std::string getRole(const std::string& username);
    bool hasPermission(const std::string& username, const std::string& permission);
};

#endif // ROLES_HPP