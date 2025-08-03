#ifndef REAL_TIME_NOTIFICATIONS_HPP
#define REAL_TIME_NOTIFICATIONS_HPP

#include <string>
#include <vector>

class RealTimeNotifications {
public:
    void subscribe(const std::string& username);
    void unsubscribe(const std::string& username);
    void notifyAll(const std::string& message);
};

#endif // REAL_TIME_NOTIFICATIONS_HPP