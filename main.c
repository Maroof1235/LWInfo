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
#include "memoryinfo.h"
#include "cpu.h"
#include "_sdl.h"


int WinMain(int argc, char* argv[])
{	
	// display all information here

	struct App* a = malloc(sizeof(struct App));
	memset(a, 0, sizeof(struct App));

	if (!SDLInit(a)) {
		AppFree(&a);
		return 1;
	}

	if (!SDLLoad(a)) {
		AppFree(&a);
		return 1;
	}

	a->is_running = true;

	SDLRun(a);

	AppFree(&a);
	return 0;
}
