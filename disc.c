#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "disc.h"

bool DiscSpaceInfo(struct DiscInfo *discinfo, uint64_t *GB, uint64_t *TB)
{
	// call function to retreive disk space information
	bool DiscResult = GetDiskFreeSpaceExA(0, 0, &discinfo->TotalBytes, &discinfo->TotalFreeBytes);

	// declare variable to hold the calculation to convert bytes to gibabytes
	uint64_t BytesToGB = 1024 * 1024 * 1024;

	// QuadPart that holds the actual 64 bit integer. Need to do this for maths operations
	// calculations
	*GB = discinfo->TotalBytes.QuadPart / BytesToGB;
	
	// present storage in terms of TB and GB; need to extract number of GBs in 2 decimal places
	*GB %= 1000;
	*GB /= 10;

	*TB = discinfo->TotalBytes.QuadPart / (BytesToGB * 1024);

	// retrieves free space in GB
	discinfo->TotalFreeBytes.QuadPart /= BytesToGB;

	return DiscResult != 0;	
}
