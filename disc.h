#pragma once
#include <windows.h>

// struct to store disk info variables that I need
struct DiscInfo {
	ULARGE_INTEGER TotalBytes;
	ULARGE_INTEGER TotalFreeBytes;
	float CPercentageSpaceUse;
	float DPercentageSpaceUse;
	uint64_t GBTotal;
	uint64_t TBTotal;
	uint64_t StoreGB;
	uint64_t InUseSpaceGB;
};

bool DiscSpaceInfo(struct DiscInfo* discinfo, const char* drive);

