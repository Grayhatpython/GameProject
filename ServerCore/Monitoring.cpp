#include "pch.h"
#include "Monitoring.h"
#include "Log.h"
#include <string>
#include <Psapi.h>

#pragma comment(lib, "pdh.lib")

Monitoring::Monitoring()
{
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	::GetSystemInfo(&sysInfo);
	_numOfProcessors = sysInfo.dwNumberOfProcessors;

	::GetSystemTimeAsFileTime(&ftime);
	::memcpy(&_lastCPU, &ftime, sizeof(FILETIME));

	_currentProcess = ::GetCurrentProcess();
	::GetProcessTimes(_currentProcess, &ftime, &ftime, &fsys, &fuser);
	::memcpy(&_lastSysCPU, &fsys, sizeof(FILETIME));
	::memcpy(&_lastUserCPU, &fuser, sizeof(FILETIME));
}

void Monitoring::Print()
{
#ifdef _DEBUG
	GThreadHistory->Print();

	std::wstring title;

	title += L"Cpu Usage : " + std::to_wstring(CpuUsage()) + L" | Virtual Memory Usage(MB) : " + std::to_wstring(ProcessVirtualMemoryUsage()) + L" | Physics Memory Usage(MB) : " + std::to_wstring(ProcessPhysMemoryUsage());
	::SetConsoleTitle(title.c_str());
#endif // _DEBUG
}

double Monitoring::CpuUsage()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent = 0.0f;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(_currentProcess, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - _lastSysCPU.QuadPart) +
		(user.QuadPart - _lastUserCPU.QuadPart);
	percent /= (now.QuadPart - _lastCPU.QuadPart);
	percent /= _numOfProcessors;
	_lastCPU = now;
	_lastUserCPU = user;
	_lastSysCPU = sys;

	return percent * 100;
}

uint64 Monitoring::ProcessVirtualMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	::GetProcessMemoryInfo(_currentProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	return pmc.PagefileUsage / 1024 / 1024;
}

uint64 Monitoring::ProcessPhysMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	::GetProcessMemoryInfo(_currentProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	return pmc.WorkingSetSize / 1024 / 1024;
}

uint64 Monitoring::TotalPhysMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);

	return memInfo.ullTotalPhys;
}

uint64 Monitoring::TotalUsedPhysMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&memInfo);

	return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
}
