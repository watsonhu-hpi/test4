#ifndef FILE_METADATA_HPP
#define FILE_METADATA_HPP

#include <string>
#include <vector>

struct FileMetadata {
    int id;
    std::string filename;
    std::string owner;
    std::string uploadDate;
    std::string fileType;
    long fileSize;
};

class FileMetadataManager {
public:
    void addMetadata(const FileMetadata& metadata);
    std::vector<FileMetadata> filterByType(const std::string& fileType);
    std::vector<FileMetadata> filterBySize(long minSize, long maxSize);
};

#endif // FILE_METADATA_HPP