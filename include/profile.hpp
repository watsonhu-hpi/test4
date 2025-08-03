#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>

class Profile {
public:
    void updateUsername(const std::string& oldUsername, const std::string& newUsername);
    void updatePassword(const std::string& username, const std::string& newPassword);
    void viewActivity(const std::string& username);
};

#endif // PROFILE_HPP