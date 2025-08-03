#include "../include/notifications.hpp"
#include <iostream>
#include <map>
#include <vector>

static std::map<std::string, std::vector<std::string>> notificationDatabase;

void Notifications::sendNotification(const std::string& username, const std::string& message) {
    notificationDatabase[username].push_back(message);
    std::cout << "Notification sent to " << username << ": " << message << std::endl;
}

std::vector<std::string> Notifications::getNotifications(const std::string& username) {
    if (notificationDatabase.find(username) != notificationDatabase.end()) {
        return notificationDatabase[username];
    }
    return {};
}