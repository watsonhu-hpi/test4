#include "../include/file_upload.hpp"
#include <iostream>

static std::vector<File> fileDatabase;

bool FileUpload::uploadFile(const std::string& username, const std::string& filename) {
    File newFile = {static_cast<int>(fileDatabase.size() + 1), username, filename, "2025-08-03"};
    fileDatabase.push_back(newFile);
    std::cout << "File uploaded successfully." << std::endl;
    return true;
}

std::vector<File> FileUpload::viewFiles(const std::string& username) {
    std::vector<File> userFiles;
    for (const auto& file : fileDatabase) {
        if (file.username == username) {
            userFiles.push_back(file);
        }
    }
    return userFiles;
}

bool FileUpload::deleteFile(int fileId, const std::string& username) {
    for (auto it = fileDatabase.begin(); it != fileDatabase.end(); ++it) {
        if (it->id == fileId && it->username == username) {
            fileDatabase.erase(it);
            std::cout << "File deleted successfully." << std::endl;
            return true;
        }
    }
    std::cout << "File not found or unauthorized." << std::endl;
    return false;
}