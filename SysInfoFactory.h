#ifndef SYSINFOMONITOR_SYSINFOFACTORY_H
#define SYSINFOMONITOR_SYSINFOFACTORY_H

#include <memory>
#include <string>
#include "SysInfo.h"
#include "LinuxInfo.h"
#include "WindowsInfo.h"

#ifdef _WIN32
#define PLATFORM_NAME "Windows"
#elif __linux__
#define PLATFORM_NAME "Linux"
#else
#define PLATFORM_NAME "Unknown"
#endif

class SysInfoFactory {
public:
    static std::unique_ptr<SysInfo> createSysInfo() {
        std::string platform = PLATFORM_NAME;

        if (platform == "Linux") {
            return std::make_unique<LinuxInfo>(platform);
        } else if (platform == "Windows") {
            return std::make_unique<WindowsInfo>(platform);
        } else {
            throw std::runtime_error("Unsupported platform");
        }
    }
};

#endif
