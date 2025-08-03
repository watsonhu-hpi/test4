#include "../include/advanced_analytics.hpp"
#include <iostream>
#include <map>

static std::map<std::string, std::map<std::string, int>> userActionCounts;

void AdvancedAnalytics::trackUserAction(const std::string& username, const std::string& action) {
    userActionCounts[username][action]++;
    std::cout << "Tracked action for " << username << ": " << action << std::endl;
}

std::map<std::string, int> AdvancedAnalytics::getUserActionCounts(const std::string& username) {
    if (userActionCounts.find(username) != userActionCounts.end()) {
        return userActionCounts[username];
    }
    return {};
}