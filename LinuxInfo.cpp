#include "LinuxInfo.h"

uint64_t LinuxInfo::getRAMTotal() {

    sysinfo(&si);

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
