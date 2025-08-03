#ifndef EXPORT_HPP
#define EXPORT_HPP

#include <string>

class Export {
public:
    void exportUsersToCSV(const std::string& filename);
    void exportCommentsToCSV(const std::string& filename);
    void exportFilesToCSV(const std::string& filename);
};

#endif // EXPORT_HPP