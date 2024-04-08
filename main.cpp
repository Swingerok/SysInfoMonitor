#include <iostream>
#include "SysInfo.h"
#include "LinuxInfo.h"
#include "WindowsInfo.h"

using namespace std;

int main() {

    LinuxInfo inf;

    cout << inf.getCurrentTasksNum() << endl;
    cout << inf.getSystemUptime() << endl;
    cout << inf.getTotalCPUUsage() << endl;
    auto vec = inf.getCPUCoresUsage();
    for (auto & i : vec) {
        cout << i << endl;
    }

    return 0;
}
