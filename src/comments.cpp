#include "../include/comments.hpp"
#include <iostream>

static std::vector<Comment> commentDatabase;

bool Comments::addComment(const std::string& username, const std::string& content) {
    Comment newComment = {static_cast<int>(commentDatabase.size() + 1), username, content};
    commentDatabase.push_back(newComment);
    std::cout << "Comment added successfully." << std::endl;
    return true;
}

std::vector<Comment> Comments::viewComments() {
    return commentDatabase;
}

bool Comments::deleteComment(int commentId, const std::string& username) {
    for (auto it = commentDatabase.begin(); it != commentDatabase.end(); ++it) {
        if (it->id == commentId && it->username == username) {
            commentDatabase.erase(it);
            std::cout << "Comment deleted successfully." << std::endl;
            return true;
        }
    }
    std::cout << "Comment not found or unauthorized." << std::endl;
    return false;
}