#include "LinuxInfo.h"

#include <utility>
#include <netdb.h>

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

        if (Str::starts_with((*it), "processor")) {
            CPUInfo::Core core{(uint64_t) Str::getint(*it)};
            while (!Str::starts_with((*it), "power management")) {
                if (Str::starts_with((*it), "model name")) {
                    cpuInfo.model = Str::getval(*it);
                } else if (Str::starts_with((*it), "physical id")) {
                    core.physicalId = (uint64_t) Str::getint(*it);
                } else if (Str::starts_with((*it), "cpu MHz")) {
                    core.speed = Str::getdouble(*it);
                } else if (Str::starts_with((*it), "cache size")) {
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

SysInfo::DriveInfo getDriveInfo(const std::string &name) {
    SysInfo::DriveInfo info;

    info.name = name;

    std::ifstream file("/sys/block/" + name + "/device/model");
    std::string line;

    if (file.is_open()) {
        std::getline(file, line);
        info.model = line;
    }
    file.close();

    file.open("/sys/block/" + name + "/size");

    if (file.is_open()) {
        std::getline(file, line);
        info.size = std::stoi(line); // bytes
        info.size *= 512;
    }
    file.close();

    return std::move(info);
}

std::vector<LinuxInfo::DriveInfo> LinuxInfo::getDrivesInfo() {
    std::vector<DriveInfo> drives;

    std::ifstream file("/proc/partitions");
    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Пропускаем оглавление
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string major, minor, blocks, name;
            if (iss >> major >> minor >> blocks >> name) {
                if (std::filesystem::exists(
                        "/sys/block/" + name + "/device")) { // пропускаем разделы, не являющиеся физическими дисками
                    drives.push_back(getDriveInfo(name));
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Ошибка открытия /proc/partitions" << std::endl;
    }

    return drives;
}

std::vector<LinuxInfo::NetworkAdapterInfo> LinuxInfo::getNetworkAdaptersInfo() {

    std::vector<NetworkAdapterInfo> vec;

    struct ifaddrs *interfaces, *ifa;
    NetworkAdapterInfo adapter;
    int family;
    std::string name;
    std::string address;
    std::string netmask;
    std::string broadcast;
    char host[NI_MAXHOST];

    // Получение списка интерфейсов
    if (getifaddrs(&interfaces) == -1) {
        perror("getifaddrs");
        return {};
    }

    for (ifa = interfaces; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        family = ifa->ifa_addr->sa_family;

        name = ifa->ifa_name;

        // Обработка IPv4 и IPv6 адресов
        if (family == AF_INET || family == AF_INET6) {
            int s = getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST,
                                nullptr, 0, NI_NUMERICHOST);
            if (s != 0) {
                std::cerr << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
                continue;
            }

            address = host;
        } else {
            continue;
        }

        // Получение маски подсети
        if (ifa->ifa_netmask) {
            int s = getnameinfo(ifa->ifa_netmask,
                                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST,
                                nullptr, 0, NI_NUMERICHOST);
            if (s != 0) {
                std::cerr << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
                continue;
            }
            netmask = host;
        } else {
            continue;
        }

        // Получение адреса вещания (только для IPv4)
        if (family == AF_INET && ifa->ifa_broadaddr) {
            int s = getnameinfo(ifa->ifa_broadaddr,
                                sizeof(struct sockaddr_in),
                                host, NI_MAXHOST,
                                nullptr, 0, NI_NUMERICHOST);
            if (s != 0) {
                std::cerr << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
                continue;
            }

            broadcast = host;
        } else {
            continue;
        }

        vec.push_back({family, name, address, netmask, broadcast});
    }

    freeifaddrs(interfaces);

    return std::move(vec);
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
        Str::erase(str1, ' ');
    }
    file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::ifstream file2("/proc/stat");

    if (file2.is_open()) {
        getline(file2, str2);
        Str::erase(str2, ' ');
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
        while (Str::starts_with(temp, "cpu")) {
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
        while (Str::starts_with(temp, "cpu")) {
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

    double work = work_2 - work_1;
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

uint64_t LinuxInfo::read_net_stat(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << path << std::endl;
        return 0;
    }
    uint64_t value;
    file >> value;
    return value;
}

uint64_t LinuxInfo::print_interface_speed(const std::string &name) {
    std::string base_path = "/sys/class/net/";
    std::string path = base_path + name;

    return read_net_stat(path);
}

uint64_t LinuxInfo::getNetworkAdapterCurrentUpload(std::string adapterName) {

    return print_interface_speed(adapterName + "/statistics/tx_bytes");
}

uint64_t LinuxInfo::getNetworkAdapterCurrentDownload(std::string adapterName) {
    return print_interface_speed(adapterName + "/statistics/rx_bytes");
}

std::string LinuxInfo::getOSFamily() {
    return "Linux";
}
