#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>

class Database {
public:
    bool initialize();
    bool executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> fetchResults(const std::string& query);
};

#endif // DATABASE_HPP