/* problem domain to decompose:
info program that displays CPU information and RAM, disk management
CPU - temperature, different cores maybe, */

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
	struct DiscInfo discinfo = { 0 };

	uint64_t GBTotal;
	uint64_t Terabyte;

	uint64_t TotalGBPhysAmount;
	uint64_t TotalMBPhysAmount;
	uint64_t AvailGBPhysAmount;
	uint64_t AvailMBPhysAmount;
	uint64_t RemainingGB;
	uint64_t RemainingMB;
	uint32_t PercentageMemUse;

	while (true)
	{
		if (MemoryInfo(&TotalGBPhysAmount, &AvailGBPhysAmount, &TotalMBPhysAmount, &AvailMBPhysAmount, &RemainingGB, &RemainingMB, &PercentageMemUse))
		{
			printf("Percentage of memory in use: %d%%\n", PercentageMemUse);
			printf("Total physical memory: %llu.%llu GB\n", TotalGBPhysAmount, TotalMBPhysAmount);
			printf("Available memory: %llu.%llu GB\n", AvailGBPhysAmount, AvailMBPhysAmount);
			printf("Memory in use %llu.%llu GB\n", RemainingGB, RemainingMB);
			printf("\n");
		
		}

		if (DiscSpaceInfo(&discinfo, &GBTotal, &Terabyte))
		{
			printf("Total size on disk (D:): %llu.%llu TB\n", Terabyte, GBTotal);
			printf("Total free space (D:): %llu GB\n", discinfo.TotalFreeBytes.QuadPart);
		}

		Sleep(1500);
		system("cls");

	}




}