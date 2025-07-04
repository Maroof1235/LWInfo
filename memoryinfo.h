#pragma once

struct MemInfo {
	uint64_t GBPhys;
	uint64_t MBPhys;
	uint64_t AvailGBPhys;
	uint64_t AvailMBPhys;
	uint64_t RemainingGB;
	uint64_t RemainingMB;
	// need uint32 for this I think
	uint32_t PercentageMemUse;
	ULONGLONG TotalPhysRAM;
};

bool MemoryInfo(struct MemInfo* meminfo);
