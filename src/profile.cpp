#include "../include/profile.hpp"
#include <iostream>

void Profile::updateUsername(const std::string& oldUsername, const std::string& newUsername) {
    std::cout << "Updating username from " << oldUsername << " to " << newUsername << std::endl;
    // Simulate logic to update username
}

void Profile::updatePassword(const std::string& username, const std::string& newPassword) {
    std::cout << "Updating password for user: " << username << std::endl;
    // Simulate logic to update password
}

void Profile::viewActivity(const std::string& username) {
    std::cout << "Viewing activity for user: " << username << std::endl;
    // Simulate logic to view user activity
}