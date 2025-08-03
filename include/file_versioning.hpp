#ifndef FILE_VERSIONING_HPP
#define FILE_VERSIONING_HPP

#include <string>
#include <vector>

struct FileVersion {
    int versionNumber;
    std::string filename;
    std::string uploadDate;
};

class FileVersioning {
public:
    void addVersion(const std::string& filename);
    std::vector<FileVersion> getFileVersions(const std::string& filename);
};

#endif // FILE_VERSIONING_HPP