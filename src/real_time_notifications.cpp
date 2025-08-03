#include "../include/real_time_notifications.hpp"
#include <iostream>
#include <set>

static std::set<std::string> subscribers;

void RealTimeNotifications::subscribe(const std::string& username) {
    subscribers.insert(username);
    std::cout << username << " subscribed to real-time notifications." << std::endl;
}

void RealTimeNotifications::unsubscribe(const std::string& username) {
    subscribers.erase(username);
    std::cout << username << " unsubscribed from real-time notifications." << std::endl;
}

void RealTimeNotifications::notifyAll(const std::string& message) {
    for (const auto& subscriber : subscribers) {
        std::cout << "Notifying " << subscriber << ": " << message << std::endl;
    }
}