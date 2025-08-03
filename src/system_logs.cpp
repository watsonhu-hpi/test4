#include "../include/system_logs.hpp"
#include <iostream>
#include <vector>

static std::vector<std::string> systemLogDatabase;

void SystemLogs::logEvent(const std::string& event) {
    systemLogDatabase.push_back(event);
    std::cout << "Logged event: " << event << std::endl;
}

std::vector<std::string> SystemLogs::getLogs() {
    return systemLogDatabase;
}