#include "WindowsInfo.h"

#include <utility>

uint64_t WindowsInfo::getRAMTotal() {
    return 0;
}

uint32_t WindowsInfo::getCPUCoresNum() {
    return 0;
}

std::string WindowsInfo::getOSVersion() {
    return std::string();
}

std::string WindowsInfo::getOSArch() {
    return std::string();
}

std::string WindowsInfo::getCPUModel() {
    return std::string();
}

std::string WindowsInfo::getCPUArch() {
    return std::string();
}

std::string WindowsInfo::getVAModel() {
    return std::string();
}

uint64_t WindowsInfo::getVRAMTotal() {
    return 0;
}

std::vector<SysInfo::DriveInfo> WindowsInfo::getDrivesInfo() {
    return std::vector<DriveInfo>();
}

std::vector<SysInfo::NetworkAdapterInfo> WindowsInfo::getNetworkAdaptersInfo() {
    return std::vector<NetworkAdapterInfo>();
}

double WindowsInfo::getSystemUptime() {
    return 0;
}

uint64_t WindowsInfo::getRAMFree() {
    return 0;
}

uint64_t WindowsInfo::getRAMUsed() {
    return 0;
}

uint32_t WindowsInfo::getTotalCPUUsage() {
    return 0;
}

std::vector<uint32_t> WindowsInfo::getCPUCoresUsage() {
    return std::vector<uint32_t>();
}

int WindowsInfo::getCurrentTasksNum() {
    return 0;
}

uint64_t WindowsInfo::getDriveCurrentWrite(std::string driveName) {
    return 0;
}

uint64_t WindowsInfo::getDriveCurrentRead(std::string driveName) {
    return 0;
}

uint64_t WindowsInfo::getNetworkAdapterCurrentUpload(std::string adapterName) {
    return 0;
}

uint64_t WindowsInfo::getNetworkAdapterCurrentDownload(std::string adapterName) {
    return 0;
}

std::string WindowsInfo::getOSFamily() {
    return OSFamily;
}

WindowsInfo::WindowsInfo(std::string OSFamily) : SysInfo(std::move(OSFamily)) {

}
