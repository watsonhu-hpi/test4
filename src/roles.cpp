#include "../include/roles.hpp"
#include <iostream>
#include <map>

static std::map<std::string, std::string> userRoles;
static std::map<std::string, std::vector<std::string>> rolePermissions = {
    {"admin", {"manage_users", "manage_comments", "manage_files"}},
    {"user", {"submit_comment", "upload_file"}}
};

void Roles::assignRole(const std::string& username, const std::string& role) {
    userRoles[username] = role;
    std::cout << "Assigned role " << role << " to user " << username << std::endl;
}

std::string Roles::getRole(const std::string& username) {
    if (userRoles.find(username) != userRoles.end()) {
        return userRoles[username];
    }
    return "guest";
}

bool Roles::hasPermission(const std::string& username, const std::string& permission) {
    std::string role = getRole(username);
    if (rolePermissions.find(role) != rolePermissions.end()) {
        auto permissions = rolePermissions[role];
        return std::find(permissions.begin(), permissions.end(), permission) != permissions.end();
    }
    return false;
}