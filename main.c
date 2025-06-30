/* problem domain to decompose:
info program that displays CPU information and RAM, disk management
CPU - temperature, different cores maybe, */

// TODO: GUI windows. First sort out functionality

// all errors enabled, so do this to minimise window error warnings that I cannot control
#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>
#include <stdlib.h>

// personal header includes
#include "disc.h"
#include "memoryinfo.h"
#include "cpu.h"

void DisplayCPUInfo(void);
void DisplayMemoryInfo(void);
void DisplayDiscInfo(void);

int main(void)
{
	// display all information here

	// TODO: need to include escaping the program, for now force close to end program
	while (true)
	{
		// CPU INFO GOES HERE
		DisplayCPUInfo();
		printf("\n");
		DisplayMemoryInfo();
		printf("\n");
		DisplayDiscInfo();

		//// to update the data
		Sleep(1500);
		system("cls");

	}
}

void DisplayCPUInfo(void)
{

	SYSTEM_INFO SysInfo;

	struct CPUTime CpuTime;

	GetCpuInfo(&SysInfo, &CpuTime);

	printf("Page size: %lu Bytes\n", SysInfo.dwPageSize);
	printf("Minimum virtual application address: 0x%p\n", SysInfo.lpMinimumApplicationAddress);
	printf("Maximum virtual application address: 0x%p\n", SysInfo.lpMaximumApplicationAddress);
	printf("Logical Processors: %lu\n", SysInfo.dwNumberOfProcessors);


	uint32_t type = SysInfo.dwProcessorType;

	switch (type)
	{
	case 386:
	{
		printf("Processor Type: i386 (%lu)\n", type);
	} break;

	case 486:
	{
		printf("Processor Type: i486 (%lu)\n", type);
	} break;

	case 586:
	{
		printf("Processor Type: i586 (Pentium) (%lu)\n", type);
	} break;

	case 2200:
	{
		printf("Processor Type: iA64 (%lu)\n", type);
	} break;

	case 8664:
	{
		printf("Processor Type: x86-64 (AMD) (%lu)\n", type);
	} break;

	default:
		break;

	}

	printf("How many processors active using mask (ffff = 16): %llx\n", SysInfo.dwActiveProcessorMask);

}

void DisplayMemoryInfo(void)
{

	struct MemInfo meminfo = { 0 };

	if (MemoryInfo(&meminfo))
	{

		printf("INSTALLED PHYSICAL RAM: %llu GB\n", meminfo.TotalPhysRAM);
		printf("Percentage of memory in use: %d%%\n", meminfo.PercentageMemUse);
		printf("Total physical memory: %llu.%llu GB\n", meminfo.GBPhys, meminfo.MBPhys);
		printf("Available memory: %llu.%llu GB\n", meminfo.AvailGBPhys, meminfo.AvailMBPhys);
		printf("Memory in use %llu.%.2llu GB\n", meminfo.RemainingGB, meminfo.RemainingMB);
	}
}

void DisplayDiscInfo(void)
{
	struct DiscInfo discinfo = { 0 };


	if (DiscSpaceInfo(&discinfo, "C:\\"))
	{
		printf("Total size on disk (C:): %llu.%llu TB\n", discinfo.TBTotal, discinfo.GBTotal);
		printf("Total free space (C:): %llu GB\n", discinfo.TotalFreeBytes.QuadPart);
		printf("Space in use (C:): %llu GB\n", discinfo.InUseSpaceGB);

		discinfo.PercentageSpaceUse = ((float)discinfo.InUseSpaceGB / discinfo.StoreGB) * 100;
		printf("Percentage of storage in use (C:): %.2f%%\n", discinfo.PercentageSpaceUse);
	}
	else
	{
		printf("Failed to retrieve C: drive info\n");
	}

	printf("\n");

	if (DiscSpaceInfo(&discinfo, "D:\\"))
	{
		printf("Total size on disk (D:): %llu.%llu TB\n", discinfo.TBTotal, discinfo.GBTotal);
		printf("Total free space (D:): %llu GB\n", discinfo.TotalFreeBytes.QuadPart);
		printf("Space in use (D:): %llu GB\n", discinfo.InUseSpaceGB);

		discinfo.PercentageSpaceUse = ((float)discinfo.InUseSpaceGB / discinfo.StoreGB) * 100;
		printf("Percentage of storage in use (D:): %.2f%%\n", discinfo.PercentageSpaceUse);
	}
	else
	{
		printf("Failed to retrieve D: drive info\n");
	}

}