#ifndef COMMENTS_HPP
#define COMMENTS_HPP

#include <string>
#include <vector>

struct Comment {
    int id;
    std::string username;
    std::string content;
};

class Comments {
public:
    bool addComment(const std::string& username, const std::string& content);
    std::vector<Comment> viewComments();
    bool deleteComment(int commentId, const std::string& username);
};

#endif // COMMENTS_HPP