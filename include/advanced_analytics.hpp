#ifndef ADVANCED_ANALYTICS_HPP
#define ADVANCED_ANALYTICS_HPP

#include <string>
#include <map>

class AdvancedAnalytics {
public:
    void trackUserAction(const std::string& username, const std::string& action);
    std::map<std::string, int> getUserActionCounts(const std::string& username);
};

#endif // ADVANCED_ANALYTICS_HPP