#ifndef SYSINFOMONITOR_SYSINFOFACTORY_H
#define SYSINFOMONITOR_SYSINFOFACTORY_H

#include <memory>
#include <string>
#include "SysInfo.h"

#ifdef WIN32
#define PLATFORM_NAME "Windows"
#include "WindowsInfo.h"
static std::unique_ptr<SysInfo> createSysInfo() {
    std::string platform = PLATFORM_NAME;
    return std::make_unique<WindowsInfo>(platform);
};
#elif __linux__
#define PLATFORM_NAME "Linux"
#include "LinuxInfo.h"
static std::unique_ptr<SysInfo> createSysInfo() {
    std::string platform = PLATFORM_NAME;
    return std::make_unique<LinuxInfo>(platform);
};
#else
#define PLATFORM_NAME "Unknown"
static std::unique_ptr<SysInfo> createSysInfo() {
    std::string platform = PLATFORM_NAME;
    throw std::runtime_error("Unsupported platform");
};
#endif

class SysInfoFactory {
public:
    static std::unique_ptr<SysInfo> createSysInfo() {
        return ::createSysInfo();
    }
};

#endif