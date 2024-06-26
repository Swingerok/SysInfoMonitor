#pragma once
#include <windows.h>
#include <iostream>
#include <string.h>
#include <intrin.h>
#include <vector>
#include <VersionHelpers.h>
#include <tlhelp32.h>
#include <sstream>

#define _WIN32_DCOM
#include <tchar.h>
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")
#include <urlmon.h>
#include <winternl.h>

#define SystemProcessorTimes 8
#define MAX_PROCESSORS 32

#include <wininet.h>
#include <chrono>
#include "SysInfo.h"

using namespace std;

class WindowsInfo : public SysInfo {

public:

    explicit WindowsInfo(std::string OSFamily);

    std::string getOSFamily() override;

    uint64_t getRAMTotal() override; //!< Пусть все функции занимаемой памяти возвращают размер в байтах пока что
	uint32_t getCPUCoresNum() override;

    std::string getOSVersion() override;
	std::string getOSArch() override; //!< x86/x86_64/ARM/...
	std::string getCPUModel() override;
    std::string getCPUArch() override; //!< x86/x86_64/ARM/...

	std::string getVAModel() override; //!< VideoAdapter
	uint64_t getVRAMTotal() override;

	std::vector<DriveInfo> getDrivesInfo() override;

	std::vector<NetworkAdapterInfo> getNetworkAdaptersInfo() override;

	//! Функции получения динамической информации о ресурсах системы
	double getSystemUptime() override;	//!< время в формате double (целая часть - секунды, вещественная - остальное)

	uint64_t getRAMFree() override;
    uint64_t getRAMUsed() override;

    uint32_t getTotalCPUUsage() override; //!< Общая от 0 до 100
	std::vector <uint32_t> getCPUCoresUsage() override; //!< по ядрам от 0 до 100
    int getCurrentTasksNum() override; //!< текущее количество запущенных процессов

	uint64_t getDriveCurrentWrite(std::string driveName) override;
	uint64_t getDriveCurrentRead(std::string driveName) override;

    uint64_t getNetworkAdapterCurrentUpload(std::string adapterName) override;
	uint64_t getNetworkAdapterCurrentDownload(std::string adapterName) override;

private:

};