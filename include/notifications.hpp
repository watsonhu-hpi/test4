#ifndef NOTIFICATIONS_HPP
#define NOTIFICATIONS_HPP

#include <string>
#include <vector>

class Notifications {
public:
    void sendNotification(const std::string& username, const std::string& message);
    std::vector<std::string> getNotifications(const std::string& username);
};

#endif // NOTIFICATIONS_HPP