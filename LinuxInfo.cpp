#include "LinuxInfo.h"

uint64_t LinuxInfo::getRAMTotal() {
    return si.totalram;
}

uint64_t LinuxInfo::getRAMFree() {
    sysinfo(&si);
    return si.freeram;
}

uint64_t LinuxInfo::getRAMUsed() {
    sysinfo(&si);
    return si.totalram - si.freeram;
}

uint32_t LinuxInfo::getCPUCoresNum() {
    return 0;
}

LinuxInfo::LinuxInfo() : SysInfo() {
    sysinfo(&si);

    std::ifstream file("/proc/cpuinfo");

    std::vector<std::string> lines;

    if (file.is_open()) {

        std::string line;

        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }
    file.close();

    auto it = lines.begin();

    while (it != lines.end()) {
        (*it).erase(remove_if((*it).begin(), (*it).end(), isspace), (*it).end());

        int i = std::stoi(*it);

        std::cout << i << std::endl;

        it++;
    }

}

std::string LinuxInfo::getOSVersion() {
    return std::string();
}

std::string LinuxInfo::getOSArch() {
    return std::string();
}

std::string LinuxInfo::getCPUModel() {
    return std::string();
}

std::string LinuxInfo::getCPUArch() {
    return std::string();
}

std::string LinuxInfo::getVAModel() {
    return std::string();
}

uint64_t LinuxInfo::getVRAMTotal() {
    return 0;
}

std::vector<LinuxInfo::DriveInfo> LinuxInfo::getDrivesInfo() {
    return std::vector<DriveInfo>();
}

std::vector<LinuxInfo::NetworkAdapterInfo> LinuxInfo::getNetworkAdaptersInfo() {
    return std::vector<NetworkAdapterInfo>();
}

double LinuxInfo::getSystemUptime() {
    return 0;
}

uint32_t LinuxInfo::getTotalCPUUsage() {
    return 0;
}

std::vector<uint32_t> LinuxInfo::getCPUCoresUsage() {
    return std::vector<uint32_t>();
}

int LinuxInfo::getCurrentTasksNum() {
    return 0;
}

uint64_t LinuxInfo::getDriveCurrentWrite(std::string driveName) {
    return 0;
}

uint64_t LinuxInfo::getDriveCurrentRead(std::string driveName) {
    return 0;
}

uint64_t LinuxInfo::getNetworkAdapterCurrentUpload(std::string adapterName) {
    return 0;
}

uint64_t LinuxInfo::getNetworkAdapterCurrentDownload(std::string adapterName) {
    return 0;
}
