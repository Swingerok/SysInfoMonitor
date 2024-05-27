#include "WindowsInfo.h"

#include <utility>

uint64_t WindowsInfo::getRAMTotal() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (float)statex.ullTotalPhys;
}

uint32_t WindowsInfo::getCPUCoresNum() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

std::string WindowsInfo::getOSVersion() {
	if (IsWindows10OrGreater())
	{
		return "Windows 10";
	}
	if (IsWindows8Point1OrGreater())
	{
		return "Windows 8.1";
	}
	if (IsWindows8OrGreater())
	{
		return "Windows 8";
	}
	if (IsWindows7OrGreater())
	{
		return "Windows 7";
	}
	if (IsWindowsVistaOrGreater())
	{
		return "Windows Vista";
	}
	if (IsWindowsXPOrGreater())
	{
		return "Windows XP";
	}
	return "Unknown";
}

std::string WindowsInfo::getOSArch() {
	#if _WIN32
	#if defined(_WIN64)
		return "x86_64";
	#else
		return "x86";
	#endif
	#elif __ANDROID__
		return "ARM";
	#else
		return "unknown";
	#endif
}

std::string WindowsInfo::getCPUModel() {
    return GetCpuVendorString();
}

std::string WindowsInfo::getCPUArch() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	if (sysinfo.wProcessorArchitecture == 9)
	{
		return "x86_64";
	}
	else if (sysinfo.wProcessorArchitecture == 5)
	{
		return "ARM";
	}
	else if (sysinfo.wProcessorArchitecture == 12)
	{
		return "ARM64";
	}
	else if (sysinfo.wProcessorArchitecture == 6)
	{
		return "Intel Itanium";
	}
	else if (sysinfo.wProcessorArchitecture == 0)
	{
		return "x86";
	}
	else if (sysinfo.wProcessorArchitecture == 0xffff)
	{
		return "Unknown architecture";
	}
}

std::string WindowsInfo::getVAModel() {
    return std::string();
}

uint64_t WindowsInfo::getVRAMTotal() {
    return 0;
}

std::vector<SysInfo::DriveInfo> WindowsInfo::getDrivesInfo() {
    return std::vector<DriveInfo>();
}

std::vector<SysInfo::NetworkAdapterInfo> WindowsInfo::getNetworkAdaptersInfo() {
    return std::vector<NetworkAdapterInfo>();
}

double WindowsInfo::getSystemUptime() {
	SYSTEMTIME sysTime;
	//Retrieves data so that we have a way to Get it to output when using the pointers
	GetSystemTime(&sysTime);
	return (double)(sysTime.wHour + ((double)sysTime.wMinute / 100));
}

uint64_t WindowsInfo::getRAMFree() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return (float)statex.ullAvailPhys;
}

uint64_t WindowsInfo::getRAMUsed() {
    return getRAMTotal() - getRAMFree();
}

uint32_t WindowsInfo::getTotalCPUUsage() {
	static char cpu[MAX_PATH] = { 0 };

	typedef NTSTATUS(NTAPI* pfNtQuerySystemInformation) (
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID SystemInformation,
		IN ULONG SystemInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

	static pfNtQuerySystemInformation NtQuerySystemInformation = NULL;

	if (NtQuerySystemInformation == NULL)
	{

		// Initializing a string object
		string Str = "ntdll.dll";

		// Initializing an object of wstring
		wstring temp = wstring(Str.begin(), Str.end());
		// Applying c_str() method on temp
		LPCWSTR wideString = temp.c_str();

		HMODULE ntDLL = ::GetModuleHandle(wideString);
		NtQuerySystemInformation = (pfNtQuerySystemInformation)GetProcAddress(ntDLL, "NtQuerySystemInformation");
	}

	typedef struct
		_SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
		LARGE_INTEGER IdleTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER Reserved1[2];
		ULONG Reserved2;
	} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;


	static SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION lastInfo = { 0 };
	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION curInfo = { 0 };

	ULONG retsize;

	NtQuerySystemInformation(SystemProcessorPerformanceInformation, &lastInfo, sizeof(lastInfo), &retsize);
	Sleep(1000);
	NtQuerySystemInformation(SystemProcessorPerformanceInformation, &curInfo, sizeof(curInfo), &retsize);
	double cpuUsage = -1;
	cpuUsage = 100.0 - double(curInfo.IdleTime.QuadPart - lastInfo.IdleTime.QuadPart) /
		double(curInfo.KernelTime.QuadPart - lastInfo.KernelTime.QuadPart + curInfo.UserTime.QuadPart - lastInfo.UserTime.QuadPart) * 100.0;
	_itoa_s(cpuUsage, cpu, 10);

	lastInfo = curInfo;

	return cpuUsage;
}

std::vector<uint32_t> WindowsInfo::getCPUCoresUsage() {
    return std::vector<uint32_t>();
}

int WindowsInfo::getCurrentTasksNum() {
	PROCESSENTRY32 Process;
	HANDLE hProcess;
	Process.dwSize = sizeof(PROCESSENTRY32);

	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int countProc = 1;
	if (Process32First(hProcess, &Process))
	{
		do
		{
			countProc++;
		} while (Process32Next(hProcess, &Process));
	}
	return countProc;
}

uint64_t WindowsInfo::getDriveCurrentWrite(std::string driveName) {
    return 0;
}

uint64_t WindowsInfo::getDriveCurrentRead(std::string driveName) {
    return 0;
}

uint64_t WindowsInfo::getNetworkAdapterCurrentUpload(std::string adapterName) {
    return 0;
}

uint64_t WindowsInfo::getNetworkAdapterCurrentDownload(std::string adapterName) {
    return 0;
}

std::string WindowsInfo::getOSFamily() {
	#if __linux__
		return "linux";
	#elif _WIN32
	#if defined(_WIN64)
		return "win64";
	#else
		return "win32";
	#endif
	#elif __APPLE__
		return "mac";
	#elif __sun
		return "solaris";
	#elif __unix__
		return "unix";
	#elif BSD
		return "bsd";
	#else
		return "unknown";
	#endif
}

WindowsInfo::WindowsInfo(std::string OSFamily) : SysInfo(std::move(OSFamily)) {

}
