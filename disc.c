#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "disc.h"

bool DiscSpaceInfo(struct DiscInfo *discinfo, const char* drive)
{ 
	// call function to retreive disk space information
	bool DiscResult = GetDiskFreeSpaceExA(drive, 0, &discinfo->TotalBytes, &discinfo->TotalFreeBytes);


	// declare variable to hold the calculation to convert bytes to gibabytes
	uint64_t BytesToGB = 1024 * 1024 * 1024;

	// QuadPart that holds the actual 64 bit integer. Need to do this for maths operations
	// calculations
	discinfo->GBTotal = discinfo->TotalBytes.QuadPart / BytesToGB;

	// store in separate variable to use in percentage calculation (feels like bad practice, review later)
	discinfo->StoreGB = discinfo->GBTotal;

	// retrieves free space in GB
	discinfo->TotalFreeBytes.QuadPart /= BytesToGB;

	// retrieves remaining space and percentage before operations on values
	discinfo->InUseSpaceGB = discinfo->GBTotal - discinfo->TotalFreeBytes.QuadPart;
	
	// present storage in terms of TB and GB; need to extract number of GBs in 2 decimal places
	discinfo->GBTotal %= 1000;
	discinfo->GBTotal /= 10;

	discinfo->TBTotal = discinfo->TotalBytes.QuadPart / (BytesToGB * 1024);

	return DiscResult != 0;
}

