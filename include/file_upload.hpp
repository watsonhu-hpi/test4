#ifndef FILE_UPLOAD_HPP
#define FILE_UPLOAD_HPP

#include <string>
#include <vector>

struct File {
    int id;
    std::string username;
    std::string filename;
    std::string uploadDate;
};

class FileUpload {
public:
    bool uploadFile(const std::string& username, const std::string& filename);
    std::vector<File> viewFiles(const std::string& username);
    bool deleteFile(int fileId, const std::string& username);
};

#endif // FILE_UPLOAD_HPP