#include "WindowsInfo.h"

#include <utility>
#pragma hdrstop

#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#include <atlbase.h>
#include <comutil.h>

#ifndef	ID_DEDICATED
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>
#endif
using namespace ATL;
namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
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

SysInfo::DriveInfo HDD(char DiscLitera)
{
	bool Flag;
	string dl = patch::to_string(DiscLitera); // �������� ������ �����
	string path = patch::to_string(DiscLitera) + ":\\";
	SysInfo::DriveInfo info;
	// ����� ����� ������ �� ����������
	WORD OldErrorMode;
	OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS); // ������� ����� ������
	bool ready = dirExists(path); // ������c� ������� ������s�� ����������
	SetErrorMode(OldErrorMode); // ��������������� ������ ����� ������ ������

	info.name = (string)"Drive " + dl;
	if (ready)
	{
		UINT drive_type = GetDriveType(((dl + ":\\").c_str())); // ����� ��� �����
		if (drive_type == DRIVE_REMOVABLE) info.model = "REMOVABLE";
		else if (drive_type == DRIVE_FIXED)	 info.model = "FIXED";
		else if (drive_type == DRIVE_REMOTE)	info.model = "REMOTE";
		else if (drive_type == DRIVE_CDROM)	 info.model = "CD-ROM";
		else if (drive_type == DRIVE_RAMDISK)   info.model = "RAMDISK";
		else info.model = "UNKNOWN TYPE";

		// ���� ��� HDD - ������������ ���������� � ���
		if (drive_type == DRIVE_FIXED)
		{
			unsigned __int64 FreeBytesAvailable;
			unsigned __int64 TotalNumberOfBytes;
			unsigned __int64 TotalNumberOfFreeBytes;
			char drive_label[30];
			char drive_fat[30];
			DWORD drive_sn;
			DWORD drive_name_size = sizeof(drive_label);

			// �������� ������ � ��������
			Flag = ::GetDiskFreeSpaceEx((path.c_str()),
				(PULARGE_INTEGER)&FreeBytesAvailable,
				(PULARGE_INTEGER)&TotalNumberOfBytes,
				(PULARGE_INTEGER)&TotalNumberOfFreeBytes
			);
			if (Flag)
			{
				info.size = TotalNumberOfBytes;
			}
		}
	}
	return info;
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
	DISPLAY_DEVICEA dd;
	dd.cb = sizeof(DISPLAY_DEVICEA);
	EnumDisplayDevicesA(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
	return string(dd.DeviceString);
	/*
	HRESULT hres;
	string strFromBstr;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) return 0;
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hres)) {
		CoUninitialize();
		return 0;
}
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres)) {
		CoUninitialize();
		return 0;
	}
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t(L"root\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres)) {
		pLoc->Release();
		CoUninitialize();
		return 0;
	}
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 0;
	}
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_VideoController"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hres)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 0;
	}
	IWbemClassObject* pclsObj;
	ULONG uReturn = 0;
	cout << "VA info:" << "\n";
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn)break;
		VARIANT vtProp;
		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		string strFromBstr = (const char*)_bstr_t(V_BSTR(&vtProp));
		VariantClear(&vtProp);
	}
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	CoUninitialize();
	return strFromBstr;
	*/
}

uint64_t WindowsInfo::getVRAMTotal() {
	//cout << "1\n";
#ifdef	ID_DEDICATED
	return 0;
#else
	unsigned int retSize = 64;
	//cout << "2\n";
	CComPtr<IWbemLocator> spLoc = NULL;
	HRESULT hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_SERVER, IID_IWbemLocator, (LPVOID*)&spLoc);
	if (hr != S_OK || spLoc == NULL) {
		//cout << hr;
		//cout << "3\n";
		return retSize;
	}
	//cout << "4\n";
	CComBSTR bstrNamespace(_T("\\\\.\\root\\CIMV2"));
	CComPtr<IWbemServices> spServices;
	//cout << "5\n";
	// Connect to CIM
	hr = spLoc->ConnectServer(bstrNamespace, NULL, NULL, 0, NULL, 0, 0, &spServices);
	if (hr != WBEM_S_NO_ERROR) {
		cout << "6\n";
		return retSize;
	}
	//cout << "7\n";
	// Switch the security level to IMPERSONATE so that provider will grant access to system-level objects.  
	hr = CoSetProxyBlanket(spServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (hr != S_OK) {
		return retSize;
	}

	// Get the vid controller
	CComPtr<IEnumWbemClassObject> spEnumInst = NULL;
	hr = spServices->CreateInstanceEnum(CComBSTR("Win32_VideoController"), WBEM_FLAG_SHALLOW, NULL, &spEnumInst);
	if (hr != WBEM_S_NO_ERROR || spEnumInst == NULL) {
		return retSize;
	}

	ULONG uNumOfInstances = 0;
	CComPtr<IWbemClassObject> spInstance[2];
	hr = spEnumInst->Next(10000, 2, (IWbemClassObject**)spInstance, &uNumOfInstances);
	//cout << "2\n";
	if (hr == S_OK && spInstance) {
		// Get properties from the object
		CComVariant varSize;
		for (int i = 0; i < uNumOfInstances; i++)
		{
			hr = spInstance[i]->Get(CComBSTR(_T("AdapterRAM")), 0, &varSize, 0, 0);
			if (hr == S_OK) {
				retSize = varSize.intVal / (1024 * 1024);
				if (retSize == 0) {
					retSize = 64;
				}
			}
		}
	}
	//cout << "1\n";
	return retSize / (1024);
#endif
}

std::vector<SysInfo::DriveInfo> WindowsInfo::getDrivesInfo() {
	vector<SysInfo::DriveInfo> vecDrives = {};
	char buf[26];
	GetLogicalDriveStringsA(sizeof(buf), buf);
	for (char* s = buf; *s; s += strlen(s) + 1)
		vecDrives.push_back(HDD(s[0]));
	/*
	for (char* s = buf; *s; s += strlen(s) + 1)
		out += HDD(s[0]);
*/
	return vecDrives;
}

std::vector<SysInfo::NetworkAdapterInfo> WindowsInfo::getNetworkAdaptersInfo() {
    return std::vector<NetworkAdapterInfo>();
}

double WindowsInfo::getSystemUptime() {
	DWORD start_time = GetTickCount();
	double elapsed_time = static_cast<double>(start_time) / 1000.0;
	return elapsed_time;

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
		string temp = string(Str.begin(), Str.end());
		// Applying c_str() method on temp
		LPCSTR wideString = temp.c_str();

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
	#if _WIN32
	#if defined(_WIN64)
		return "win64";
	#else
		return "win32";
	#endif
	#endif
}

WindowsInfo::WindowsInfo(std::string OSFamily) : SysInfo(std::move(OSFamily)) {
    ::CoInitialize(NULL);
}