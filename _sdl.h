#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

struct App {
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* text_font;

	// memory text
	SDL_Texture* MemInfoText;
	SDL_Texture* PhysRamText;
	SDL_Texture* TotalRamText;
	SDL_Texture* PercentRamText;
	SDL_Texture* MemInUseText;
	SDL_Texture* AvailRamText;
	// disc text
	SDL_Texture* DiscInfoText;
	SDL_Texture* TotalSpaceText;
	SDL_Texture* FreeSpaceText;
	SDL_Texture* SpaceInUseText;
	SDL_Texture* PercentSpaceText;
	// cpu text
	
	// FRects for text positioning
	SDL_FRect MemInfoTextRect;
	SDL_FRect PhysRamTextRect;
	SDL_FRect TotalRamTextRect;
	SDL_FRect PercentRamTextRect;
	SDL_FRect MemInUseTextRect;
	SDL_FRect AvailRamTextRect;
	// disc text rects 
	SDL_FRect DiscInfoTextRect;
	SDL_FRect TotalSpaceTextRect;
	SDL_FRect FreeSpaceTextRect;
	SDL_FRect SpaceInUseTextRect;
	SDL_FRect PercentSpaceTextRect;

	SDL_Event event;
	bool is_running;
};

bool SDLInit(struct App* a);

void AppFree(struct App** app);

bool SDLLoad(struct App* a);

void AppEvents(struct App* a);


void UpdateValues(struct App* a, struct MemInfo* meminfo);

void SDLRun(struct App* a);

