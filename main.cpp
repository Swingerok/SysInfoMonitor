#include <iostream>
#include "SysInfo.h"

using namespace std;

int main() {

    SysInfo inf;

    cout << inf.getRAMTotal() / 1024 << endl;

    return 0;
}
