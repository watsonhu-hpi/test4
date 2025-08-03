#ifndef MESSAGE_HISTORY_HPP
#define MESSAGE_HISTORY_HPP

#include <string>
#include <vector>
#include "database.hpp"

class MessageHistory {
public:
    MessageHistory(Database& db) : db(db) {}

    void storeMessage(const std::string& sender, const std::string& recipient, const std::string& message) {
        // Logic to store the message in the database
    }

    std::vector<std::string> retrieveHistory(const std::string& user1, const std::string& user2) {
        // Logic to retrieve message history between two users
        return {};
    }

private:
    Database& db;
};

#endif // MESSAGE_HISTORY_HPP