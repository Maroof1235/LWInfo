#pragma once
#include <windows.h>

// struct to store disk info variables that I need
struct DiscInfo {
	ULARGE_INTEGER TotalBytes;
	ULARGE_INTEGER TotalFreeBytes;
};

bool DiscSpaceInfo(struct DiscInfo *discinfo, uint64_t *GB, uint64_t *TB);
