#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "cpu.h"

//GetSystemInfo(DWORD.dwProcessorType);

bool GetCpuInfo(struct SYSTEM_INFO* SysInfo, struct CPUTime* CpuTime)
{

	GetSystemInfo(SysInfo);

	ULONGLONG IdleDiff, KernelDiff, UserDiff, Total;

	FILETIME IdleTime, KernelTime, UserTime;

	SYSTEMTIME ConvertedIdleTime, ConvertedKernelTime, ConvertedUserTime;

	GetSystemTimes(&IdleTime, &KernelTime, &UserTime);

	CpuTime->PrevIdle.LowPart = IdleTime.dwLowDateTime;
	CpuTime->PrevIdle.HighPart = IdleTime.dwHighDateTime;

	CpuTime->PrevKernel.LowPart = KernelTime.dwLowDateTime;
	CpuTime->PrevKernel.HighPart = KernelTime.dwHighDateTime;

	CpuTime->PrevUser.LowPart = UserTime.dwLowDateTime;
	CpuTime->PrevUser.HighPart = UserTime.dwHighDateTime;


	Sleep(1000);
	GetSystemTimes(&IdleTime, &KernelTime, &UserTime);

	CpuTime->Idle.LowPart = IdleTime.dwLowDateTime;
	CpuTime->Idle.HighPart = IdleTime.dwHighDateTime;

	CpuTime->Kernel.LowPart = KernelTime.dwLowDateTime;
	CpuTime->Kernel.HighPart = KernelTime.dwHighDateTime;

	CpuTime->User.LowPart = UserTime.dwLowDateTime;
	CpuTime->User.HighPart = UserTime.dwHighDateTime;

	IdleDiff = CpuTime->Idle.QuadPart - CpuTime->PrevIdle.QuadPart;
	KernelDiff = CpuTime->Kernel.QuadPart - CpuTime->PrevKernel.QuadPart;
	UserDiff = CpuTime->User.QuadPart - CpuTime->PrevUser.QuadPart;

	Total = (UserDiff + KernelDiff);
	double CpuUsage;

	CpuUsage = (double)(Total - IdleDiff) / Total * 100;

	printf("CPU USAGE: %.f%%\n", CpuUsage);
	
	return true;
}
