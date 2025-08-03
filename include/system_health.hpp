#ifndef SYSTEM_HEALTH_HPP
#define SYSTEM_HEALTH_HPP

#include <string>

class SystemHealth {
public:
    void checkCPUUsage();
    void checkMemoryUsage();
    void checkDiskSpace();
};

#endif // SYSTEM_HEALTH_HPP