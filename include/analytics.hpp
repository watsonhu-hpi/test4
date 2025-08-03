#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <string>
#include <map>

class Analytics {
public:
    void trackEvent(const std::string& eventName);
    std::map<std::string, int> getEventCounts();
};

#endif // ANALYTICS_HPP