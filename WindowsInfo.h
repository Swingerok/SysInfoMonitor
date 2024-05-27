#ifndef UNTITLED1_WINDOWSINFO_H
#define UNTITLED1_WINDOWSINFO_H
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

namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}

//for drives
bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

//for str => LPCWSTR
LPCWSTR stringToLPCWSTR(string a)
{
	// Initializing an object of wstring
	wstring temp = wstring(a.begin(), a.end());

	// Applying c_str() method on temp
	LPCWSTR wideString = temp.c_str();
	return wideString;
}

//for char => LPWSTR
LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}

//for vendor
namespace
{
	struct cpuid_regs
	{
		DWORD   Eax;
		DWORD   Ebx;
		DWORD   Ecx;
		DWORD   Edx;
	};
	std::string SplitIntoChars(DWORD Value)
	{
		std::string Str;
		char const* pCursor = (char const*)&Value;
		for (int i = 0; i < sizeof(Value); ++i) {
			Str += pCursor[i];
		}
		return Str;
	}
	std::string GetCpuVendorSubstring(DWORD Eax)
	{
		cpuid_regs Regs;
		__cpuid((int*)&Regs, Eax);
		std::string Str;
		Str += SplitIntoChars(Regs.Eax);
		Str += SplitIntoChars(Regs.Ebx);
		Str += SplitIntoChars(Regs.Ecx);
		Str += SplitIntoChars(Regs.Edx);
		return Str;
	}
	std::string GetCpuVendorString()
	{
		std::string VendorString;
		cpuid_regs Regs;
		__cpuid((int*)&Regs, 0x80000000);
		if (Regs.Eax >= 0x80000004)
		{
			VendorString =
				GetCpuVendorSubstring(0x80000002) +
				GetCpuVendorSubstring(0x80000003) +
				GetCpuVendorSubstring(0x80000004);
		}
		return VendorString;
	}
}

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

#endif