#include "../include/activity.hpp"
#include <iostream>
#include <vector>
#include <map>

static std::map<std::string, std::vector<std::string>> activityLog;

void Activity::logActivity(const std::string& username, const std::string& action) {
    activityLog[username].push_back(action);
    std::cout << "Logged activity for " << username << ": " << action << std::endl;
}

std::vector<std::string> Activity::getActivityLog(const std::string& username) {
    if (activityLog.find(username) != activityLog.end()) {
        return activityLog[username];
    }
    return {};
}