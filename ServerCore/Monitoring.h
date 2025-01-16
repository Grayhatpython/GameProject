#pragma once

class Monitoring
{
public:
	Monitoring();

public:
	void Print();

private:
	double	CpuUsage();
	//	Mega Bytes
	uint64	ProcessVirtualMemoryUsage();	
	uint64	ProcessPhysMemoryUsage();

	//	Bytes
	uint64	TotalPhysMemory();
	uint64	TotalUsedPhysMemory();

private:
	ULARGE_INTEGER	_lastCPU;
	ULARGE_INTEGER	_lastSysCPU;
	ULARGE_INTEGER	_lastUserCPU;
	int32			_numOfProcessors = 0;
	HANDLE			_currentProcess = INVALID_HANDLE_VALUE;;
};

