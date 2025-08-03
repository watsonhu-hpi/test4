#ifndef SYSTEM_LOGS_HPP
#define SYSTEM_LOGS_HPP

#include <string>
#include <vector>

class SystemLogs {
public:
    void logEvent(const std::string& event);
    std::vector<std::string> getLogs();
};

#endif // SYSTEM_LOGS_HPP