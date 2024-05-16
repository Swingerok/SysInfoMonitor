#include <iostream>
#include "SysInfo.h"
#include "LinuxInfo.h"

using namespace std;

int main() {

    LinuxInfo inf;

    vector<SysInfo::DriveInfo> drives = inf.getDrivesInfo();

    vector<SysInfo::NetworkAdapterInfo> adapters = inf.getNetworkAdaptersInfo();

    for (const auto & adapter : adapters) {
        cout << "Adapter name: " << adapter.name << endl;
        cout << "Adapter address: " << adapter.address << endl;
        cout << "Adapter netmask: " << adapter.netmask << endl;
        cout << "Adapter broadcast: " << adapter.broadcast << endl;
        cout << "Adapter current upload: " << inf.getNetworkAdapterCurrentUpload(adapter.name) << endl;
        cout << "Adapter current download: " << inf.getNetworkAdapterCurrentDownload(adapter.name) << endl;
        cout << "----------------------------------------------------" << endl;
    }



    cout << "Ram total: " << inf.getRAMTotal() << endl;

    cout << "Cpu cores num: " << inf.getCPUCoresNum() << endl;

    cout << "OS Version: " << inf.getOSVersion() << endl;



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
