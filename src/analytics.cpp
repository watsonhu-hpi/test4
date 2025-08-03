#include "../include/analytics.hpp"
#include <iostream>
#include <map>

static std::map<std::string, int> eventCounts;

void Analytics::trackEvent(const std::string& eventName) {
    eventCounts[eventName]++;
    std::cout << "Tracked event: " << eventName << std::endl;
}

std::map<std::string, int> Analytics::getEventCounts() {
    return eventCounts;
}