#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <string>
#include <vector>

class Search {
public:
    std::vector<std::string> searchComments(const std::string& keyword);
    std::vector<std::string> searchFiles(const std::string& keyword);
};

#endif // SEARCH_HPP