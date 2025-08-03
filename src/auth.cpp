#include "../include/auth.hpp"
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, std::string> userDatabase;

bool Auth::registerUser(const std::string& username, const std::string& password) {
    if (userDatabase.find(username) != userDatabase.end()) {
        std::cout << "User already exists." << std::endl;
        return false;
    }
    userDatabase[username] = password;
    std::cout << "User registered successfully." << std::endl;
    return true;
}

bool Auth::loginUser(const std::string& username, const std::string& password) {
    if (userDatabase.find(username) == userDatabase.end() || userDatabase[username] != password) {
        std::cout << "Invalid username or password." << std::endl;
        return false;
    }
    currentUser = username;
    std::cout << "User logged in successfully." << std::endl;
    return true;
}

void Auth::logoutUser() {
    currentUser.clear();
    std::cout << "User logged out successfully." << std::endl;
}