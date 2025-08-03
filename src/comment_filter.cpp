#include "../include/comment_filter.hpp"
#include <iostream>
#include <vector>

static std::vector<Comment> commentDatabase = {
    {1, "user1", "This is a test comment."},
    {2, "user2", "Another comment for testing."},
    {3, "user1", "Yet another test comment."}
};

std::vector<Comment> CommentFilter::filterByKeyword(const std::string& keyword) {
    std::vector<Comment> filtered;
    for (const auto& comment : commentDatabase) {
        if (comment.content.find(keyword) != std::string::npos) {
            filtered.push_back(comment);
        }
    }
    return filtered;
}

std::vector<Comment> CommentFilter::filterByUser(const std::string& username) {
    std::vector<Comment> filtered;
    for (const auto& comment : commentDatabase) {
        if (comment.username == username) {
            filtered.push_back(comment);
        }
    }
    return filtered;
}