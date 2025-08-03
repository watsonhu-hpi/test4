#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <string>
#include <vector>

class Admin {
public:
    void viewAllUsers();
    void deleteUser(const std::string& username);
    void viewAllComments();
    void deleteComment(int commentId);
    void viewAllFiles();
    void deleteFile(int fileId);
};

#endif // ADMIN_HPP