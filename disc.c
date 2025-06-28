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
	//discinfo->TotalBytes.QuadPart = 0;
	//discinfo->TotalFreeBytes.QuadPart = 0;


	bool DiscResult = GetDiskFreeSpaceExA(0, 0, &discinfo->TotalBytes, &discinfo->TotalFreeBytes);

	// declare variable to hold the calculation to convert bytes to gibabytes
	uint64_t BytesToGB = 1024 * 1024 * 1024;

	// QuadPart that holds the actual 64 bit integer. Need to do this for maths operations
	// calculations
	*GB = discinfo->TotalBytes.QuadPart / BytesToGB;
	
	// truncate last 3 digits
	*GB = *GB % 1000;

	*TB = discinfo->TotalBytes.QuadPart / (BytesToGB * 1024);

	// retrieves free space in GB
	discinfo->TotalFreeBytes.QuadPart = discinfo->TotalFreeBytes.QuadPart / BytesToGB;

	if (DiscResult != 0)
	{
		return true;

	}
	else
	{
		return false;
	}

}
