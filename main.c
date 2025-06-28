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

// personal header includes
#include "disc.h"




int main(void)
{
	struct DiscInfo discinfo = { 0 };

	uint64_t GBTotal;
	uint64_t Terabyte;
	if (DiscSpaceInfo(&discinfo, &GBTotal, &Terabyte))
	{
		printf("Total size on disk (D:): %llu.%llu TB\n", Terabyte, GBTotal);
		printf("Total free space (D:): %llu GB\n", discinfo.TotalFreeBytes.QuadPart);
		printf("Heloo\n");

	}

}