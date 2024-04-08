#ifndef UNTITLED1_LINUXINFO_H
#define UNTITLED1_LINUXINFO_H

#include "SysInfo.h"
#include <fstream>
#include <string>
#include <sys/utsname.h>
#include <chrono>
#include <thread>
#include "Str.h"

class LinuxInfo : public SysInfo {

public:

    LinuxInfo();

    uint64_t getRAMTotal() override; //!< Пусть все функции занимаемой памяти возвращают размер в байтах пока что +
	uint32_t getCPUCoresNum() override; // +

    std::string getOSVersion() override; // +
	std::string getOSArch() override; //!< x86/x86_64/ARM/... +
	std::string getCPUModel() override; //+
    std::string getCPUArch() override; //!< x86/x86_64/ARM/... +

	std::string getVAModel() override; //!< VideoAdapter
	uint64_t getVRAMTotal() override;

	std::vector<DriveInfo> getDrivesInfo() override;

	std::vector<NetworkAdapterInfo> getNetworkAdaptersInfo() override;

	//! Функции получения динамической информации о ресурсах системы
	double getSystemUptime() override;	//!< время в формате double (целая часть - секунды, вещественная - остальное) +

	uint64_t getRAMFree() override; // +
    uint64_t getRAMUsed() override; // +

    uint32_t getTotalCPUUsage() override; //!< Общая от 0 до 100
	std::vector <uint32_t> getCPUCoresUsage() override; //!< по ядрам от 0 до 100
    int getCurrentTasksNum() override; //!< текущее количество запущенных процессов +

	uint64_t getDriveCurrentWrite(std::string driveName) override;
	uint64_t getDriveCurrentRead(std::string driveName) override;

    uint64_t getNetworkAdapterCurrentUpload(std::string adapterName) override;
	uint64_t getNetworkAdapterCurrentDownload(std::string adapterName) override;

private:
    uint32_t calculateCPUUsage(std::string str1, std::string str2);

private:
    struct sysinfo si{};
    utsname kernelInf{};
    CPUInfo cpuInfo{};
};


#endif //UNTITLED1_LINUXINFO_H
