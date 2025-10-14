#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "memoryinfo.h"

bool MemoryInfo(struct MemInfo* meminfo)
{
	
	MEMORYSTATUSEX MemStatus; 

	MemStatus.dwLength = sizeof(MemStatus);

	GlobalMemoryStatusEx(&MemStatus);
	
	// conversions
	uint64_t BytesToGB = 1024 * 1024 * 1024;
	uint64_t BytesToMB = 1024 * 1024;
	uint64_t KBToGB = 1024 * 1024;

	meminfo->GBPhys = (MemStatus.ullTotalPhys) / BytesToGB;
	meminfo->MBPhys = MemStatus.ullTotalPhys / BytesToMB;

	meminfo->MBPhys %= 1000;
	meminfo->MBPhys /= 10;

	meminfo->AvailGBPhys = MemStatus.ullAvailPhys/ BytesToGB;
	meminfo->AvailMBPhys = (MemStatus.ullAvailPhys) / BytesToMB;

	meminfo->AvailMBPhys %= 1000;
	meminfo->AvailMBPhys /= 10;
	
	uint64_t InUseBytes = MemStatus.ullTotalPhys - MemStatus.ullAvailPhys;
	meminfo->RemainingGB = InUseBytes / BytesToGB;
	meminfo->RemainingMB = (InUseBytes / BytesToMB) % 1000 / 10;

	meminfo->PercentageMemUse = MemStatus.dwMemoryLoad;

	GetPhysicallyInstalledSystemMemory(&meminfo->TotalPhysRAM);

	meminfo->TotalPhysRAM /= (KBToGB);
	

	return GlobalMemoryStatusEx != 0 && GetPhysicallyInstalledSystemMemory != 0;
}