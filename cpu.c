#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
//GetSystemInfo(DWORD.dwProcessorType);

bool GetCpuInfo(struct SYSTEM_INFO* SysInfo)
{

	GetSystemInfo(SysInfo);

	return true;
}
