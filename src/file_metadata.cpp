#include "../include/file_metadata.hpp"
#include <iostream>
#include <vector>

static std::vector<FileMetadata> metadataDatabase;

void FileMetadataManager::addMetadata(const FileMetadata& metadata) {
    metadataDatabase.push_back(metadata);
    std::cout << "Added metadata for file: " << metadata.filename << std::endl;
}

std::vector<FileMetadata> FileMetadataManager::filterByType(const std::string& fileType) {
    std::vector<FileMetadata> filtered;
    for (const auto& metadata : metadataDatabase) {
        if (metadata.fileType == fileType) {
            filtered.push_back(metadata);
        }
    }
    return filtered;
}

std::vector<FileMetadata> FileMetadataManager::filterBySize(long minSize, long maxSize) {
    std::vector<FileMetadata> filtered;
    for (const auto& metadata : metadataDatabase) {
        if (metadata.fileSize >= minSize && metadata.fileSize <= maxSize) {
            filtered.push_back(metadata);
        }
    }
    return filtered;
}