#include <iostream>
#include "SysInfo.h"
#include "LinuxInfo.h"
#include "WindowsInfo.h"

using namespace std;

int main() {

    LinuxInfo inf;

    std::vector<SysInfo::DriveInfo> drives = inf.getDrivesInfo();

    for (const auto & drive : drives) {
        cout << "Drive: " << drive.name << endl;
        cout << "Model: " << drive.model << endl;
        cout << "Size: " << drive.size / 1024 / 1024 / 1024 << " Gb" << endl;
        cout << "-----------------------" << endl;
    }

    cout << "Current tasks number: " << inf.getCurrentTasksNum() << endl;
    cout << "System uptime: " << inf.getSystemUptime() << endl;
    auto vec = inf.getCPUCoresUsage();
    for (auto &i: vec) {
        cout << "Core usage: " << i << endl;
    }
    cout << "Total CPU usage: " << inf.getTotalCPUUsage() << endl;
    cout << "VRAM total: " << inf.getVRAMTotal() << endl;
    cout << "VA Model: " << inf.getVAModel() << endl;

    return 0;
}
