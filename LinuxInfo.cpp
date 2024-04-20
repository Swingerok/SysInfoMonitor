#include "LinuxInfo.h"

#include <utility>



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

    uname(&kernelInf);

    cpuInfo.arch = kernelInf.machine;

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
            CPUInfo::Core core{(uint64_t) Str::getint(*it)};
            while (!(*(it)).starts_with("power management")) {
                if ((*it).starts_with("model name")) {
                    cpuInfo.model = Str::getval(*it);
                } else if ((*it).starts_with("physical id")) {
                    core.physicalId = (uint64_t) Str::getint(*it);
                } else if ((*it).starts_with("cpu MHz")) {
                    core.speed = Str::getdouble(*it);
                } else if ((*it).starts_with("cache size")) {
                    core.cacheSize = Str::getint(*it);
                }
                it++;
            }
            cpuInfo.cores.push_back(core);
        }

        it++;
    }

}

std::string LinuxInfo::getOSVersion() {
    return kernelInf.version;
}

std::string LinuxInfo::getOSArch() {
    return cpuInfo.arch;
}

std::string LinuxInfo::getCPUModel() {
    return cpuInfo.model;
}

std::string LinuxInfo::getCPUArch() {
    return cpuInfo.arch;
}

std::string LinuxInfo::getVAModel() {

    if (!gpu_model.empty()) {
        return gpu_model;
    }

    // Инициализация библиотеки libpci
    auto *pacc = pci_alloc();
    pci_init(pacc);
    pci_scan_bus(pacc);


    // Поиск видеокарты
    for (auto pdev = pacc->devices; pdev; pdev = pdev->next) {
        pci_fill_info(pdev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);
        if (pdev->device_class == PCI_CLASS_DISPLAY_VGA) {
            char vendor_name[1024];
            pci_lookup_name(pacc, vendor_name, sizeof(vendor_name), PCI_LOOKUP_VENDOR, pdev->vendor_id);
            gpu_model = vendor_name;
        }
    }

    // Освобождение ресурсов
    pci_cleanup(pacc);

    return gpu_model;
}

uint64_t LinuxInfo::getVRAMTotal() {
    GLint totalMem;
    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMem);
    return totalMem;
}

std::vector<LinuxInfo::DriveInfo> LinuxInfo::getDrivesInfo() {
    return std::vector<DriveInfo>();
}

std::vector<LinuxInfo::NetworkAdapterInfo> LinuxInfo::getNetworkAdaptersInfo() {
    return std::vector<NetworkAdapterInfo>();
}

double LinuxInfo::getSystemUptime() {
    std::ifstream file("/proc/uptime");

    std::string str;

    if (file.is_open()) {
        std::getline(file, str);
    }
    file.close();

    return std::stod(str);
}

uint32_t LinuxInfo::getTotalCPUUsage() {
    std::ifstream file("/proc/stat");

    std::string str1;
    std::string str2;

    if (file.is_open()) {
        getline(file, str1);
        str1.erase(std::find(str1.begin(), str1.end(), ' '));
    }
    file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::ifstream file2("/proc/stat");

    if (file2.is_open()) {
        getline(file2, str2);
        str2.erase(std::find(str2.begin(), str2.end(), ' '));
    }
    file2.close();

    return calculateCPUUsage(str1, str2);
}

std::vector<uint32_t> LinuxInfo::getCPUCoresUsage() {
    std::ifstream file("/proc/stat");

    std::vector<std::string> vec1;
    std::vector<std::string> vec2;

    std::string temp;

    if (file.is_open()) {
        getline(file, temp);
        getline(file, temp);
        while (temp.starts_with("cpu")) {
            vec1.push_back(temp);
            getline(file, temp);
        }
    }
    file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::ifstream file2("/proc/stat");

    if (file2.is_open()) {
        getline(file2, temp);
        getline(file2, temp);
        while (temp.starts_with("cpu")) {
            vec2.push_back(temp);
            getline(file2, temp);
        }
    }
    file2.close();

    std::vector<uint32_t> res;

    for (int i = 0; i < vec1.size(); i++) {
        res.push_back(calculateCPUUsage(vec1[i], vec2[i]));
    }

    return res;
}

uint32_t LinuxInfo::calculateCPUUsage(std::string str1, std::string str2) {

    std::vector<std::string> first = Str::split(std::move(str1));

    std::vector<std::string> second = Str::split(std::move(str2));

    double total_1 = 0;
    double total_2 = 0;
    double work_1 = 0;
    double work_2 = 0;

    for (int i = 1; i < first.size(); i++) {
        total_1 += std::stod(first[i]);
        total_2 += std::stod(second[i]);
        if (i <= 3) {
            work_1 += std::stod(first[i]);
            work_2 += std::stod(second[i]);
        }
    }

    double work =  work_2 - work_1;
    double total = total_2 - total_1;

    return (work / total) * 100;
}

int LinuxInfo::getCurrentTasksNum() {
    sysinfo(&si);
    return si.procs;
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
