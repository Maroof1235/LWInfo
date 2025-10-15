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

	// CPU USAGE
	ULONGLONG IdleDiff, KernelDiff, UserDiff, Total;

	FILETIME IdleTime, KernelTime, UserTime;

	SYSTEMTIME ConvertedIdleTime, ConvertedKernelTime, ConvertedUserTime;

	if (CpuTime->IsFirstRun)
	{
		// get times
		GetSystemTimes(&IdleTime, &KernelTime, &UserTime);

		CpuTime->PrevIdle.LowPart = IdleTime.dwLowDateTime;
		CpuTime->PrevIdle.HighPart = IdleTime.dwHighDateTime;

		CpuTime->PrevKernel.LowPart = KernelTime.dwLowDateTime;
		CpuTime->PrevKernel.HighPart = KernelTime.dwHighDateTime;

		CpuTime->PrevUser.LowPart = UserTime.dwLowDateTime;
		CpuTime->PrevUser.HighPart = UserTime.dwHighDateTime;

		CpuTime->IsFirstRun = false;

		CpuTime->CpuUsage = 0.0;

		return false;

	}
	
	CpuTime->PrevIdle = CpuTime->Idle;
	CpuTime->PrevKernel = CpuTime->Kernel;
	CpuTime->PrevUser = CpuTime->User;

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

	if (Total > 0) {
		CpuTime->CpuUsage = (double)(Total - IdleDiff) / Total * 100;
	} 
	else {
		CpuTime->CpuUsage = 0.0;
	}
	return true;
}
