#include <iostream>

#include "SysInfoFactory.h"

using namespace std;

int main() {

    std::unique_ptr<SysInfo> inf = SysInfoFactory::createSysInfo();

    vector<SysInfo::DriveInfo> drives = inf->getDrivesInfo();

    vector<SysInfo::NetworkAdapterInfo> adapters = inf->getNetworkAdaptersInfo();

    cout << "OSFamily: " << inf->getOSFamily() << endl;

    cout << "OS Version: " << inf->getOSVersion() << endl;

    cout << "OS Arch: " << inf->getOSArch() << endl;

    cout << "CPU Model: " << inf->getCPUModel() << endl;

    cout << "CPU Arch: " << inf->getCPUArch() << endl;

    cout << "Cpu cores num: " << inf->getCPUCoresNum() << endl;

    cout << "VA Model: " << inf->getVAModel() << endl;

    cout << "Video RAM Total: " << inf->getVRAMTotal() / 1024 << " Mb" << endl;

    cout << "SystemUpTime: " << inf->getSystemUptime() << " Seconds" << endl;

    cout << "Ram total: " << inf->getRAMTotal() / (1024 * 1024) << " Mb" << endl;

    cout << "RAM Free: " << inf->getRAMFree() / (1024 * 1024) << " Mb"  << endl;

    cout << "RAM Used: " << inf->getRAMUsed() / (1024 * 1024) << " Mb"  << endl;

    cout << "Total CPU Usage: " << inf->getTotalCPUUsage() << "%" << endl;

    auto vec = inf->getCPUCoresUsage();

    for (auto &i: vec) {
        cout << "Core usage: " << i << "%" << endl;
    }

    cout << "Current Tasks Num: " << inf->getCurrentTasksNum() << endl;

    cout << "----------------------------------------------------" << endl;

    for (const auto & adapter : adapters) {
        cout << "Adapter name: " << adapter.name << endl;
        cout << "Adapter address: " << adapter.address << endl;
        cout << "Adapter netmask: " << adapter.netmask << endl;
        cout << "Adapter broadcast: " << adapter.broadcast << endl;
        cout << "Adapter current upload: " << inf->getNetworkAdapterCurrentUpload(adapter.name) / 1024 << " Kb" << endl;
        cout << "Adapter current download: " << inf->getNetworkAdapterCurrentDownload(adapter.name) / 1024 << " Kb" << endl;
        cout << "----------------------------------------------------" << endl;
    }

    for (const auto & drive : drives) {
        cout << "Drive: " << drive.name << endl;
        cout << "Model: " << drive.model << endl;
        cout << "Size: " << drive.size / 1024 / 1024 / 1024 << " Gb" << endl;
        cout << "-----------------------" << endl;
    }

    return 0;
}
