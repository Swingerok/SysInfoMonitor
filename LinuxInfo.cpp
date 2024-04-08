#include "LinuxInfo.h"

auto ltrim = [](std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
};

auto getint = [](std::string str) {

    str.erase(std::remove_if(
                      str.begin(),
                      str.end(),
                      [](char ch) {
                          return !std::isdigit(ch);
                      }),
              str.end()
    );
    return std::stoi(str);
};

auto getdouble = [](std::string str) {

    str.erase(std::remove_if(
                      str.begin(),
                      str.end(),
                      [](char ch) {
                          if (ch == '.') {
                              return false;
                          }
                          return !std::isdigit(ch);
                      }),
              str.end()
    );
    return std::stod(str);
};

auto getval = [](std::string str) {

    int i = 0;
    do {
        i++;
    } while (str[i] != ':');
    std::string res = str.substr(i + 1, str.size());
    res.shrink_to_fit();
    return res;
};

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
    return cpuInfo.cores.size();
}

LinuxInfo::LinuxInfo() : SysInfo() {
    sysinfo(&si);

    utsname kernelInfo{};

    uname(&kernelInfo);

    cpuInfo.arch = kernelInfo.machine;

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

        if ((*it).starts_with("processor")) {
            CPUInfo::Core core{(uint64_t) getint(*it)};
            while (!(*(it)).starts_with("power management")) {
                if ((*it).starts_with("model name")) {
                    cpuInfo.model = ltrim(getval(*it));
                } else if ((*it).starts_with("physical id")) {
                    core.physicalId = (uint64_t) getint(*it);
                } else if ((*it).starts_with("cpu MHz")) {
                    core.speed = getdouble(*it);
                } else if ((*it).starts_with("cache size")) {
                    core.cacheSize = (uint64_t) getint(*it);
                }
                it++;
            }
            cpuInfo.cores.push_back(core);
        }

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
    return cpuInfo.model;
}

std::string LinuxInfo::getCPUArch() {
    return cpuInfo.arch;
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
