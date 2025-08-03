#ifndef CHAT_MANAGER_HPP
#define CHAT_MANAGER_HPP

#include <string>
#include <vector>
#include "database.hpp"
#include "notifications.hpp"

class ChatManager {
public:
    ChatManager(Database& db, Notifications& notifications) : db(db), notifications(notifications) {}

    void sendMessage(const std::string& sender, const std::string& recipient, const std::string& message) {
        // Logic to store the message in the database
        notifications.sendNotification(recipient, "New message from " + sender);
    }

    std::vector<std::string> getMessageHistory(const std::string& user1, const std::string& user2) {
        // Logic to fetch message history between two users from the database
        return {};
    }

private:
    Database& db;
    Notifications& notifications;
};

#endif // CHAT_MANAGER_HPP