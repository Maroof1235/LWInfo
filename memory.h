#pragma once

struct MemInfo {
	uint64_t GBPhys;
	uint64_t MBPhys;
	uint64_t AvailGBPhys;
	uint64_t AvailMBPhys;
	uint64_t RemainingGB;
	uint64_t RemainingMB;
	uint32_t PercentageMemUse;
};

bool MemoryInfo(struct MemInfo* meminfo);
