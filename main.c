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
#include "memory.h"

int main(void)
{
	struct MemInfo meminfo = { 0 };
	struct DiscInfo discinfo = { 0 };

	// TODO: need to include escaping the program, for now force close to end program
	while (true)
	{
		if (MemoryInfo(&meminfo))
		{
			printf("Percentage of memory in use: %d%%\n", meminfo.PercentageMemUse);
			printf("Total physical memory: %llu.%llu GB\n", meminfo.GBPhys, meminfo.MBPhys);
			printf("Available memory: %llu.%llu GB\n", meminfo.AvailGBPhys, meminfo.AvailMBPhys);
			printf("Memory in use %llu.%.2llu GB\n", meminfo.RemainingGB, meminfo.RemainingMB);
			printf("\n");
		
		}

		if (DiscSpaceInfo(&discinfo, "C:\\"))
		{
			printf("Total size on disk (C:): %llu.%llu TB\n", discinfo.TBTotal, discinfo.GBTotal);
			printf("Total free space (C:): %llu GB\n", discinfo.TotalFreeBytes.QuadPart);
			printf("Space in use (C:): %llu GB\n", discinfo.InUseSpaceGB);
			
			discinfo.PercentageSpaceUse = ((float) discinfo.InUseSpaceGB / discinfo.StoreGB) * 100;
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

		// to update the data
		Sleep(1500);
		system("cls");

	}




}