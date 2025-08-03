#include "../include/search.hpp"
#include <iostream>
#include <vector>

std::vector<std::string> Search::searchComments(const std::string& keyword) {
    std::cout << "Searching comments for keyword: " << keyword << std::endl;
    // Simulate search logic
    return {"Comment 1", "Comment 2"};
}

std::vector<std::string> Search::searchFiles(const std::string& keyword) {
    std::cout << "Searching files for keyword: " << keyword << std::endl;
    // Simulate search logic
    return {"File 1", "File 2"};
}