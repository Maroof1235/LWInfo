#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "_sdl.h"

#include "disc.h"
#include "memoryinfo.h"

// sdl prototypes and variables

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "LWInfo"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600

#define TEXT_SIZE 20
#define HEADER_COLOUR (SDL_Color){255, 0, 0, 255}
#define TEXT_COLOUR (SDL_Color){0,0,0,255}
#define TEXT_STR "SDL"

bool SDLInit(struct App* a)
{
	if (!TTF_Init()) {
		fprintf(stderr, "Error initialising SDL3: %s\n", SDL_GetError());
		return false;
	}

	if (!SDL_Init(SDL_FLAGS)) {
		fprintf(stderr, "Error initialising SDL3_ttf: %s\n", SDL_GetError());
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


	// DISC COLUMN HEADER
	surf = TTF_RenderText_Blended(a->text_font, "Disc Information (C:)", 0, HEADER_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->DiscInfoTextRect.x = 245.0f;
	a->DiscInfoTextRect.y = 0.0f;
	a->DiscInfoTextRect.w = (float)surf->w;
	a->DiscInfoTextRect.h = (float)surf->h;
	a->DiscInfoText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->DiscInfoText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	
	SDL_Texture* PercentRamText;
	SDL_Texture* MemInUseText;

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
	

	// memory in use
	snprintf(text_buffer, sizeof(text_buffer), "RAM in use %llu.%.2llu GB", meminfo.RemainingGB, meminfo.RemainingMB);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->MemInUseTextRect.x = 0.0f;
	a->MemInUseTextRect.y = 90.0f;
	a->MemInUseTextRect.w = (float)surf->w;
	a->MemInUseTextRect.h = (float)surf->h;
	a->MemInUseText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->MemInUseText) {
		fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
		return false;
	}


	// available RAM
	snprintf(text_buffer, sizeof(text_buffer), "Available RAM: %llu.%llu GB", meminfo.AvailGBPhys, meminfo.AvailMBPhys);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);
	if (!surf) {
		fprintf(stderr, "Error rendering text to surface: %s\n", SDL_GetError());
		return false;
	}
	a->AvailRamTextRect.x = 0.0f;
	a->AvailRamTextRect.y = 120.0f;  
	a->AvailRamTextRect.w = (float)surf->w;
	a->AvailRamTextRect.h = (float)surf->h;
	a->AvailRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
	SDL_DestroySurface(surf);
	if (!a->AvailRamText) {
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

void UpdateValues(struct App* a, struct MemInfo* meminfo)
{
	// get fresh data
	if (!MemoryInfo(meminfo)) {
		return;
	}


	char text_buffer[256];
	SDL_Surface* surf = NULL;


	// mem in use
	if (a->MemInUseText) {
		SDL_DestroyTexture(a->MemInUseText);
	}

	snprintf(text_buffer, sizeof(text_buffer), "RAM in Use: %llu.%.2llu GB", meminfo->RemainingGB, meminfo->RemainingMB);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);

	if (surf) {
		a->MemInUseTextRect.w = (float)surf->w;
		a->MemInUseTextRect.h = (float)surf->h;
		a->MemInUseText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
	}


	// available ram
	if (a->AvailRamText) {
		SDL_DestroyTexture(a->AvailRamText);
	}

	snprintf(text_buffer, sizeof(text_buffer), "Available RAM: %llu.%.2llu GB", meminfo->AvailGBPhys, meminfo->AvailMBPhys);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);

	if (surf) {
		a->AvailRamTextRect.w = (float)surf->w;
		a->AvailRamTextRect.h = (float)surf->h;
		a->AvailRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
	}

	// percent ram
	if (a->PercentRamText) {
		SDL_DestroyTexture(a->PercentRamText);
	}

	snprintf(text_buffer, sizeof(text_buffer), "%d%% of RAM in use", meminfo->PercentageMemUse);
	surf = TTF_RenderText_Blended(a->text_font, text_buffer, 0, TEXT_COLOUR);

	if (surf) {
		a->PercentRamTextRect.w = (float)surf->w;
		a->PercentRamTextRect.h = (float)surf->h;
		a->PercentRamText = SDL_CreateTextureFromSurface(a->renderer, surf);
		SDL_DestroySurface(surf);
	}


}

void SDLRun(struct App* a)
{
	Uint64 LastUpdate = SDL_GetTicks();
	struct MemInfo meminfo = { 0 };

	

	while (a->is_running) {
		AppEvents(a);

		// update every second
		Uint64 Now = SDL_GetTicks();
		if (Now - LastUpdate >= 1000) {
			UpdateValues(a, &meminfo);  
			LastUpdate = Now;
		}

		SDL_SetRenderDrawColor(a->renderer, 255, 255, 255, 255);	// white background
		SDL_RenderClear(a->renderer);

		// lines to draw to make table
		SDL_SetRenderDrawColor(a->renderer, 0, 0, 0, 255);
		SDL_RenderLine(a->renderer, 0.0, 30.0, 1000.0, 30.0);	// horizontal line
		SDL_RenderLine(a->renderer, 240.0, 0.0, 240.0, 600.0);	// vertical line separating columns
		SDL_RenderLine(a->renderer, 0.0, 60.0, 1000.0, 60.0);
		SDL_RenderLine(a->renderer, 0.0, 90.0, 1000.0, 90.0);
		SDL_RenderLine(a->renderer, 0.0, 120.0, 1000.0, 120.0);
		SDL_RenderLine(a->renderer, 0.0, 150.0, 1000.0, 150.0);
		SDL_RenderLine(a->renderer, 0.0, 180.0, 1000.0, 180.0);


		// render the metrics
		SDL_RenderTexture(a->renderer, a->MemInfoText, NULL, &a->MemInfoTextRect);
		SDL_RenderTexture(a->renderer, a->PhysRamText, NULL, &a->PhysRamTextRect);
		SDL_RenderTexture(a->renderer, a->TotalRamText, NULL, &a->TotalRamTextRect);
		SDL_RenderTexture(a->renderer, a->MemInUseText, NULL, &a->MemInUseTextRect);
		SDL_RenderTexture(a->renderer, a->AvailRamText, NULL, &a->AvailRamTextRect);
		SDL_RenderTexture(a->renderer, a->PercentRamText, NULL, &a->PercentRamTextRect);
		

		SDL_RenderTexture(a->renderer, a->DiscInfoText, NULL, &a->DiscInfoTextRect);
		SDL_RenderPresent(a->renderer);
	}

}