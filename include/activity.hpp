#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include <string>
#include <vector>

class Activity {
public:
    void logActivity(const std::string& username, const std::string& action);
    std::vector<std::string> getActivityLog(const std::string& username);
};

#endif // ACTIVITY_HPP