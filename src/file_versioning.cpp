#include "../include/file_versioning.hpp"
#include <iostream>
#include <vector>
#include <map>

static std::map<std::string, std::vector<FileVersion>> fileVersionDatabase;

void FileVersioning::addVersion(const std::string& filename) {
    int versionNumber = fileVersionDatabase[filename].size() + 1;
    fileVersionDatabase[filename].push_back({versionNumber, filename, "2025-08-03"});
    std::cout << "Added version " << versionNumber << " for file: " << filename << std::endl;
}

std::vector<FileVersion> FileVersioning::getFileVersions(const std::string& filename) {
    if (fileVersionDatabase.find(filename) != fileVersionDatabase.end()) {
        return fileVersionDatabase[filename];
    }
    return {};
}