#pragma once

struct CPUTime {
	ULARGE_INTEGER Idle;
	ULARGE_INTEGER Kernel;
	ULARGE_INTEGER User;

	ULARGE_INTEGER PrevIdle;
	ULARGE_INTEGER PrevKernel;
	ULARGE_INTEGER PrevUser;

	double CpuUsage;
	bool IsFirstRun;
};

bool GetCpuInfo(struct SYSTEM_INFO* SysInfo, struct CPUTime* CpuTime);
