#include <iostream>
#include "SysInfo.h"
#include "LinuxInfo.h"
#include "WindowsInfo.h"

using namespace std;

int main() {

    LinuxInfo inf;

    //cout << inf.getCurrentTasksNum() << endl;
    //cout << inf.getSystemUptime() << endl;
    //auto vec = inf.getCPUCoresUsage();
    //for (auto &i: vec) {
    //    cout << i << endl;
    //}
    //cout << inf.getTotalCPUUsage() << endl;

    //cout << inf.getVRAMTotal() << endl;
    cout << inf.getVAModel() << endl;

    return 0;
}
