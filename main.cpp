#include <iostream>
#include "SysInfo.h"
#include "LinuxInfo.h"
#include "WindowsInfo.h"

using namespace std;

int main() {

    LinuxInfo inf;

    cout << inf.getCPUArch() << endl;
    cout << inf.getCPUModel() << endl;
    cout << inf.getCPUCoresNum() << endl;

    return 0;
}
