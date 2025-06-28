#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

bool MemoryInfo(uint64_t *GBPhys, uint64_t *AvailGBPhys, uint64_t *MBPhys, uint64_t *AvailMBPhys, uint64_t *RemainingGB, uint64_t *RemainingMB, uint32_t *Percentage)
{
	MEMORYSTATUSEX MemStatus;

	MemStatus.dwLength = sizeof(MemStatus);

	GlobalMemoryStatusEx(&MemStatus);
	
	// conversions
	uint64_t BytesToGB = 1024 * 1024 * 1024;
	uint64_t BytesToMB = 1024 * 1024;

	*GBPhys = (MemStatus.ullTotalPhys) / BytesToGB;
	*MBPhys = MemStatus.ullTotalPhys / BytesToMB;

	*MBPhys %= 1000;
	*MBPhys /= 10;

	*AvailGBPhys = MemStatus.ullAvailPhys/ BytesToGB;
	*AvailMBPhys = (MemStatus.ullAvailPhys) / BytesToMB;

	*AvailMBPhys %= 1000;
	*AvailMBPhys /= 10;

	*RemainingGB = *GBPhys - *AvailGBPhys;
	*RemainingMB = *MBPhys - *AvailMBPhys;

	*Percentage = MemStatus.dwMemoryLoad;

	//TODO: Min, Max variables to hold memory
	return GlobalMemoryStatusEx != 0;
}