#ifndef COMMENT_FILTER_HPP
#define COMMENT_FILTER_HPP

#include <string>
#include <vector>

struct Comment {
    int id;
    std::string username;
    std::string content;
};

class CommentFilter {
public:
    std::vector<Comment> filterByKeyword(const std::string& keyword);
    std::vector<Comment> filterByUser(const std::string& username);
};

#endif // COMMENT_FILTER_HPP