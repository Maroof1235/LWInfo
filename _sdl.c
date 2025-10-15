#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "_sdl.h"

#include "disc.h"
#include "memoryinfo.h"
#include "cpu.h"

// sdl prototypes and variables

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "LWInfo"
#define WINDOW_WIDTH 1100
#define WINDOW_HEIGHT 180

#define TEXT_SIZE 20
#define HEADER_COLOUR (SDL_Color){255, 0, 0, 255}
#define TEXT_COLOUR (SDL_Color){0,0,0,255}
#define TEXT_STR "SDL"

bool SDLInit(struct App* a)
{
	if (!TTF_Init()) {
		fprintf(stderr, "Error initialising SDL3_ttf: %s\n", SDL_GetError());
		return false;
	}

	if (!SDL_Init(SDL_FLAGS)) {
		fprintf(stderr, "Error initialising SDL3: %s\n", SDL_GetError());
		return false;
	}

	a->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!a->window) {
		fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
		return false;
	}

	a->renderer = SDL_CreateRenderer(a->window, NULL);
	if (!a->renderer) {
		fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
		return false;
	}

	return true;

}

void AppFree(struct App** app)
{
	if (*app) {
		struct App* a = *app;

		// close last thing loaded first 

		if (a->cpuinfo) {
			free(a->cpuinfo);
		}

		if (a->PhysRamText) {
			SDL_DestroyTexture(a->PhysRamText);
			a->PhysRamText = NULL;
		}

		if (a->text_font) {
			TTF_CloseFont(a->text_font);
			a->text_font = NULL;
		}

		if (a->renderer) {
			SDL_DestroyRenderer(a->renderer);
			a->renderer = NULL;
		}

		if (a->window) {
			SDL_DestroyWindow(a->window);
			a->window = NULL;
		}



		TTF_Quit();
		SDL_Quit();

		free(a);

		a = NULL;
		*app = NULL;
	}
}

bool SDLLoad(struct App* a)
{
	struct MemInfo meminfo = { 0 };

	struct DiscInfo discinfo = { 0 };

	//struct CPUTime cpuinfo = { 0 };

	//SYSTEM_INFO SysInfo;

	a->hasCDrive = false;
	a->hasDDrive = false;

	a->cpuinfo = malloc(sizeof(struct CPUTime));
	memset(a->cpuinfo, 0, sizeof(struct CPUTime));
	a->cpuinfo->IsFirstRun = true;
	GetCpuInfo(&a->sysinfo, a->cpuinfo);

	a->text_font = TTF_OpenFont("fonts/8bitOperatorPlus-Regular.ttf", TEXT_SIZE);
	if (!a->text_font) {
		fprintf(stderr, "Error opening font: %s\n", SDL_GetError());
		return false;
	}
	if (!MemoryInfo(&meminfo)) {
		fprintf(stderr, "Error getting memory info\n");
		return false;
	}

	char text_buffer[256];
	SDL_Surface* surf = NULL;

	// text that will not change

	// MEMORY COLUMN HEADER
	surf = TTF_RenderText_Blended(a->text_font, "Memory Information", 0, HEADER_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->MemInfoTextRect.x = 0.0f;
	a->MemInfoTextRect.y = 0.0f;
	a->MemInfoTextRect.w = (float)surf->w;
	a->MemInfoTextRect.h = (float)surf->h;
	a->MemInfoText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->MemInfoText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// DISC COLUMN HEADERs
	// C drive
	surf = TTF_RenderText_Blended(a->text_font, "Disc Information (C:)", 0, HEADER_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->CDiscInfoTextRect.x = 525.0f;
	a->CDiscInfoTextRect.y = 0.0f;
	a->CDiscInfoTextRect.w = (float)surf->w;
	a->CDiscInfoTextRect.h = (float)surf->h;
	a->CDiscInfoText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->CDiscInfoText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// D: drive header
	surf = TTF_RenderText_Blended(a->text_font, "Disc Information (D:)", 0, HEADER_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->DDiscInfoTextRect.x = 800.0f;
	a->DDiscInfoTextRect.y = 0.0f;
	a->DDiscInfoTextRect.w = (float)surf->w;
	a->DDiscInfoTextRect.h = (float)surf->h;
	a->DDiscInfoText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->DDiscInfoText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	// CPU COLUMN HEADER
	surf = TTF_RenderText_Blended(a->text_font, "CPU Information", 0, HEADER_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->CpuInfoTextRect.x = 250.0f;
	a->CpuInfoTextRect.y = 0.0f;
	a->CpuInfoTextRect.w = (float)surf->w;
	a->CpuInfoTextRect.h = (float)surf->h;
	a->CpuInfoText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->CpuInfoText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	// physically installed RAM
	snprintf(text_buffer, sizeof(text_buffer), "Installed RAM: %llu GB", meminfo.TotalPhysRAM);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->PhysRamTextRect.x = 0.0f;
	a->PhysRamTextRect.y = 30.0f;
	a->PhysRamTextRect.w = (float)surf->w;
	a->PhysRamTextRect.h = (float)surf->h;
	a->PhysRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->PhysRamText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// total RAM
	snprintf(text_buffer, sizeof(text_buffer), "Total RAM: %llu.%llu GB", meminfo.GBPhys, meminfo.MBPhys);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->TotalRamTextRect.x = 0.0f;
	a->TotalRamTextRect.y = 60.0f;  // Different Y position
	a->TotalRamTextRect.w = (float)surf->w;
	a->TotalRamTextRect.h = (float)surf->h;
	a->TotalRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->TotalRamText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	// dynamic metrics
	// 
	// available RAM
	snprintf(text_buffer, sizeof(text_buffer), "Available RAM: %llu.%llu GB", meminfo.AvailGBPhys, meminfo.AvailMBPhys);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->AvailRamTextRect.x = 0.0f;
	a->AvailRamTextRect.y = 90.0f;
	a->AvailRamTextRect.w = (float)surf->w;
	a->AvailRamTextRect.h = (float)surf->h;
	a->AvailRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->AvailRamText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// memory in use
	snprintf(text_buffer, sizeof(text_buffer), "RAM in use %llu.%02llu GB", meminfo.RemainingGB, meminfo.RemainingMB);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->MemInUseTextRect.x = 0.0f;
	a->MemInUseTextRect.y = 120.0f;
	a->MemInUseTextRect.w = (float)surf->w;
	a->MemInUseTextRect.h = (float)surf->h;
	a->MemInUseText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->MemInUseText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// percent memory
	snprintf(text_buffer, sizeof(text_buffer), "%d%% of RAM in use", meminfo.PercentageMemUse);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->PercentRamTextRect.x = 0.0f;
	a->PercentRamTextRect.y = 150.0f;
	a->PercentRamTextRect.w = (float)surf->w;
	a->PercentRamTextRect.h = (float)surf->h;
	a->PercentRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->PercentRamText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	// C: drive info
	if (DiscSpaceInfo(&discinfo, "C:\\"))
	{
		a->hasCDrive = true;

		// total disc space
		snprintf(text_buffer, sizeof(text_buffer), "Total disc size: %llu.%llu TB", discinfo.TBTotal, discinfo.GBTotal);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s", SDL_GetError());
			return false;
		}
		a->CTotalSpaceTextRect.x = 525.0f;
		a->CTotalSpaceTextRect.y = 30.0f;
		a->CTotalSpaceTextRect.w = (float)surf->w;
		a->CTotalSpaceTextRect.h = (float)surf->h;
		a->CTotalSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->CTotalSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// free space
		snprintf(text_buffer, sizeof(text_buffer), "Total free space: %llu GB", discinfo.TotalFreeBytes.QuadPart);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s", SDL_GetError());
			return false;
		}
		a->CFreeSpaceTextRect.x = 525.0f;
		a->CFreeSpaceTextRect.y = 60.0f;
		a->CFreeSpaceTextRect.w = (float)surf->w;
		a->CFreeSpaceTextRect.h = (float)surf->h;
		a->CFreeSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->CFreeSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// space in use
		snprintf(text_buffer, sizeof(text_buffer), "Space in use: %llu GB", discinfo.InUseSpaceGB);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
			return false;
		}
		a->CSpaceInUseTextRect.x = 525.0f;
		a->CSpaceInUseTextRect.y = 90.0f;
		a->CSpaceInUseTextRect.w = (float)surf->w;
		a->CSpaceInUseTextRect.h = (float)surf->h;
		a->CSpaceInUseText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->CSpaceInUseText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// percent in use
		discinfo.CPercentageSpaceUse = ((float)discinfo.InUseSpaceGB / discinfo.StoreGB) * 100;
		snprintf(text_buffer, sizeof(text_buffer), "%.2f%% of space in use", discinfo.CPercentageSpaceUse);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
			return false;
		}
		a->CPercentSpaceTextRect.x = 525.0f;
		a->CPercentSpaceTextRect.y = 120.0f;
		a->CPercentSpaceTextRect.w = (float)surf->w;
		a->CPercentSpaceTextRect.h = (float)surf->h;
		a->CPercentSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->CPercentSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		printf("Failed to retrieve C: drive info\n");
	}

	// D: drive info
	if (DiscSpaceInfo(&discinfo, "D:\\"))
	{
		a->hasDDrive = true;

		// total disc space
		snprintf(text_buffer, sizeof(text_buffer), "Total disc size: %llu.%llu TB", discinfo.TBTotal, discinfo.GBTotal);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s", SDL_GetError());
			return false;
		}
		a->DTotalSpaceTextRect.x = 800.0f;
		a->DTotalSpaceTextRect.y = 30.0f;
		a->DTotalSpaceTextRect.w = (float)surf->w;
		a->DTotalSpaceTextRect.h = (float)surf->h;
		a->DTotalSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->DTotalSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// free space
		snprintf(text_buffer, sizeof(text_buffer), "Total free space: %llu GB", discinfo.TotalFreeBytes.QuadPart);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s", SDL_GetError());
			return false;
		}
		a->DFreeSpaceTextRect.x = 800.0f;
		a->DFreeSpaceTextRect.y = 60.0f;
		a->DFreeSpaceTextRect.w = (float)surf->w;
		a->DFreeSpaceTextRect.h = (float)surf->h;
		a->DFreeSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->DFreeSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// space in use
		snprintf(text_buffer, sizeof(text_buffer), "Space in use: %llu GB", discinfo.InUseSpaceGB);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
			return false;
		}
		a->DSpaceInUseTextRect.x = 800.0f;
		a->DSpaceInUseTextRect.y = 90.0f;
		a->DSpaceInUseTextRect.w = (float)surf->w;
		a->DSpaceInUseTextRect.h = (float)surf->h;
		a->DSpaceInUseText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->DSpaceInUseText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}

		// percent in use
		discinfo.DPercentageSpaceUse = ((float)discinfo.InUseSpaceGB / discinfo.StoreGB) * 100;
		snprintf(text_buffer, sizeof(text_buffer), "%.2f%% of space in use", discinfo.DPercentageSpaceUse);
		surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
		if (!surf) {
			fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
			return false;
		}
		a->DPercentSpaceTextRect.x = 800.0f;
		a->DPercentSpaceTextRect.y = 120.0f;
		a->DPercentSpaceTextRect.w = (float)surf->w;
		a->DPercentSpaceTextRect.h = (float)surf->h;
		a->DPercentSpaceText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
		if (!a->DPercentSpaceText) {
			fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		printf("Failed to retrieve D: drive info\n");
	}

	// cpu stuff

	// cpu percent age usage
	snprintf(text_buffer, sizeof(text_buffer), "%.2f%% CPU usage", a->cpuinfo->CpuUsage);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->CpuUsageTextRect.x = 250.0f;
	a->CpuUsageTextRect.y = 30.0f;
	a->CpuUsageTextRect.w = (float)surf->w;
	a->CpuUsageTextRect.h = (float)surf->h;
	a->CpuUsageText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->CpuUsageText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	// logical processor - THIS WILL NOT CHANGE
	snprintf(text_buffer, sizeof(text_buffer), "Logical Processors: %lu", a->sysinfo.dwNumberOfProcessors);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->LogicalProcessorsTextRect.x = 250.0f;
	a->LogicalProcessorsTextRect.y = 60.0f;
	a->LogicalProcessorsTextRect.w = (float)surf->w;
	a->LogicalProcessorsTextRect.h = (float)surf->h;
	a->LogicalProcessorsText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->LogicalProcessorsText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}

	return true;
}



void AppEvents(struct App* a)
{
	while (SDL_PollEvent(&a->event)) {
		switch (a->event.type) {
		case SDL_EVENT_QUIT:
			a->is_running = false;
			break;
		default:
			break;
		}
	}
}

bool UpdateTexture(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture** texture, SDL_FRect* rect, const char* text, SDL_Color colour)
{
	SDL_Surface* surf = TTF_RenderText_Blended(font, text, 0, colour);
	if (!surf) {
		return false;
	}

	SDL_Texture* NewTexture = SDL_CreateTextureFromSurface(renderer, surf);

	rect->w = (float)surf->w;
	rect->h = (float)surf->h;

	SDL_DestroySurface(surf);

	if (NewTexture) {
		SDL_DestroyTexture(*texture);	// destroy old texture
		*texture = NewTexture;	// point to new texture
		return true;
	}

	return false;
}

void UpdateValues(struct App* a, struct MemInfo* meminfo, struct DiscInfo* discinfo)
{
	// get fresh data
	if (!MemoryInfo(meminfo)) {
		return;
	}

	GetCpuInfo(&a->sysinfo, a->cpuinfo);

	char text_buffer[256];
	SDL_Surface* surf = NULL;


	// mem in use
	snprintf(text_buffer, sizeof(text_buffer), "RAM in Use: %llu.%02llu GB", meminfo->RemainingGB, meminfo->RemainingMB);
	UpdateTexture(a->renderer, a->text_font, &a->MemInUseText, &a->MemInUseTextRect, text_buffer, TEXT_COLOUR);


	// available ram
	snprintf(text_buffer, sizeof(text_buffer), "Available RAM: %llu.%.2llu GB", meminfo->AvailGBPhys, meminfo->AvailMBPhys);
	UpdateTexture(a->renderer, a->text_font, &a->AvailRamText, &a->AvailRamTextRect, text_buffer, TEXT_COLOUR);

	// percent ram
	snprintf(text_buffer, sizeof(text_buffer), "%d%% of RAM in use", meminfo->PercentageMemUse);
	UpdateTexture(a->renderer, a->text_font, &a->PercentRamText, &a->PercentRamTextRect, text_buffer, TEXT_COLOUR);

	// disc stuff


	// C: drive updating
	if (DiscSpaceInfo(discinfo, "C:\\"))
	{
		discinfo->CPercentageSpaceUse = ((float)discinfo->InUseSpaceGB / discinfo->StoreGB) * 100;

		// total free space
		snprintf(text_buffer, sizeof(text_buffer), "Total free space: %llu GB", discinfo->TotalFreeBytes.QuadPart);
		UpdateTexture(a->renderer, a->text_font, &a->CFreeSpaceText, &a->CFreeSpaceTextRect, text_buffer, TEXT_COLOUR);
 
		// space in use
		snprintf(text_buffer, sizeof(text_buffer), "Space in use: %llu GB", discinfo->InUseSpaceGB);
		UpdateTexture(a->renderer, a->text_font, &a->CSpaceInUseText, &a->CSpaceInUseTextRect, text_buffer, TEXT_COLOUR);

		// percent use
		snprintf(text_buffer, sizeof(text_buffer), "%.2f%% of space in use", discinfo->CPercentageSpaceUse);
		UpdateTexture(a->renderer, a->text_font, &a->CPercentSpaceText, &a->CPercentSpaceTextRect, text_buffer, TEXT_COLOUR);
	}
	else
	{
		printf("Failed to retrieve C: drive info\n");
	}

	
	// D: drive updating
	if (DiscSpaceInfo(discinfo, "D:\\"))
	{
		discinfo->DPercentageSpaceUse = ((float)discinfo->InUseSpaceGB / discinfo->StoreGB) * 100;

		// total free space
		snprintf(text_buffer, sizeof(text_buffer), "Total free space: %llu GB", discinfo->TotalFreeBytes.QuadPart);
		UpdateTexture(a->renderer, a->text_font, &a->DFreeSpaceText, &a->DFreeSpaceTextRect, text_buffer, TEXT_COLOUR);

		// space in use
		snprintf(text_buffer, sizeof(text_buffer), "Space in use: %llu GB", discinfo->InUseSpaceGB);
		UpdateTexture(a->renderer, a->text_font, &a->DSpaceInUseText, &a->DSpaceInUseTextRect, text_buffer, TEXT_COLOUR);

		// percent use
		snprintf(text_buffer, sizeof(text_buffer), "%.2f%% of space in use", discinfo->DPercentageSpaceUse);
		UpdateTexture(a->renderer, a->text_font, &a->DPercentSpaceText, &a->DPercentSpaceTextRect, text_buffer, TEXT_COLOUR);
	}
	else
	{
		printf("Failed to retrieve D: drive info\n");
	}

	// cpu usage %
	snprintf(text_buffer, sizeof(text_buffer), "%.2f%% CPU usage", a->cpuinfo->CpuUsage);
	UpdateTexture(a->renderer, a->text_font, &a->CpuUsageText, &a->CpuUsageTextRect, text_buffer, TEXT_COLOUR);
}

void SDLRun(struct App* a)
{
	Uint64 LastUpdate = SDL_GetTicks();
	struct MemInfo meminfo = { 0 };
	struct DiscInfo discinfo = { 0 };
	

	while (a->is_running) {
		AppEvents(a);

		// update every second
		Uint64 Now = SDL_GetTicks();
		if (Now - LastUpdate >= 1000) {
			UpdateValues(a, &meminfo, &discinfo);  
			LastUpdate = Now;
		}

		SDL_SetRenderDrawColor(a->renderer, 255, 255, 255, 255);	// white background
		SDL_RenderClear(a->renderer);

		// lines to draw to make table
		SDL_SetRenderDrawColor(a->renderer, 0, 0, 0, 255);
		SDL_RenderLine(a->renderer, 245.0, 0.0, 245.0, 600.0);	// vertical line separating columns
		SDL_RenderLine(a->renderer, 520.0, 0.0, 520.0, 600.0);
		SDL_RenderLine(a->renderer, 795.0, 0.0, 795.0, 600.0);
		
		SDL_RenderLine(a->renderer, 0.0, 30.0, 1100.0, 30.0);	// horizontal line
		SDL_RenderLine(a->renderer, 0.0, 60.0, 1100.0, 60.0);
		SDL_RenderLine(a->renderer, 0.0, 90.0, 1100.0, 90.0);
		SDL_RenderLine(a->renderer, 0.0, 120.0, 1100.0, 120.0);
		SDL_RenderLine(a->renderer, 0.0, 150.0, 1100.0, 150.0);
		SDL_RenderLine(a->renderer, 0.0, 180.0, 1100.0, 180.0);


		// render the metrics
		SDL_RenderTexture(a->renderer, a->MemInfoText, NULL, &a->MemInfoTextRect);
		SDL_RenderTexture(a->renderer, a->PhysRamText, NULL, &a->PhysRamTextRect);
		SDL_RenderTexture(a->renderer, a->TotalRamText, NULL, &a->TotalRamTextRect);
		SDL_RenderTexture(a->renderer, a->MemInUseText, NULL, &a->MemInUseTextRect);
		SDL_RenderTexture(a->renderer, a->AvailRamText, NULL, &a->AvailRamTextRect);
		SDL_RenderTexture(a->renderer, a->PercentRamText, NULL, &a->PercentRamTextRect);


		// CPu rendering
		SDL_RenderTexture(a->renderer, a->CpuInfoText, NULL, &a->CpuInfoTextRect);
		SDL_RenderTexture(a->renderer, a->CpuUsageText, NULL, &a->CpuUsageTextRect);
		SDL_RenderTexture(a->renderer, a->LogicalProcessorsText, NULL, &a->LogicalProcessorsTextRect);

		// disc metrics
		if (a->hasCDrive)
		{
			SDL_RenderTexture(a->renderer, a->CDiscInfoText, NULL, &a->CDiscInfoTextRect);

			// C: drive rendering
			SDL_RenderTexture(a->renderer, a->CTotalSpaceText, NULL, &a->CTotalSpaceTextRect);
			SDL_RenderTexture(a->renderer, a->CFreeSpaceText, NULL, &a->CFreeSpaceTextRect);
			SDL_RenderTexture(a->renderer, a->CSpaceInUseText, NULL, &a->CSpaceInUseTextRect);
			SDL_RenderTexture(a->renderer, a->CPercentSpaceText, NULL, &a->CPercentSpaceTextRect);
		}

		if (a->hasDDrive)
		{
			SDL_RenderTexture(a->renderer, a->DDiscInfoText, NULL, &a->DDiscInfoTextRect);


			// D: drive rendering
			SDL_RenderTexture(a->renderer, a->DTotalSpaceText, NULL, &a->DTotalSpaceTextRect);
			SDL_RenderTexture(a->renderer, a->DFreeSpaceText, NULL, &a->DFreeSpaceTextRect);
			SDL_RenderTexture(a->renderer, a->DSpaceInUseText, NULL, &a->DSpaceInUseTextRect);
			SDL_RenderTexture(a->renderer, a->DPercentSpaceText, NULL, &a->DPercentSpaceTextRect);

		}

		// ~60 frames per second
		SDL_Delay(16);
	
		SDL_RenderPresent(a->renderer);
	}

}