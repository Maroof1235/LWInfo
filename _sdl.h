#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <windows.h>

// forward declarations
struct CPUTime;
struct MemInfo;
struct DiscInfo;

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
	SDL_Texture* CDiscInfoText;
	SDL_Texture* CTotalSpaceText;
	SDL_Texture* CFreeSpaceText;
	SDL_Texture* CSpaceInUseText;
	SDL_Texture* CPercentSpaceText;

	SDL_Texture* DDiscInfoText;
	SDL_Texture* DTotalSpaceText;
	SDL_Texture* DFreeSpaceText;
	SDL_Texture* DSpaceInUseText;
	SDL_Texture* DPercentSpaceText;
	// cpu text
	SDL_Texture* CpuInfoText;
	SDL_Texture* CpuUsageText;
	SDL_Texture* LogicalProcessorsText;
	
	// FRects for text positioning
	SDL_FRect MemInfoTextRect;
	SDL_FRect PhysRamTextRect;
	SDL_FRect TotalRamTextRect;
	SDL_FRect PercentRamTextRect;
	SDL_FRect MemInUseTextRect;
	SDL_FRect AvailRamTextRect;
	// disc text rects 
	SDL_FRect CDiscInfoTextRect;
	SDL_FRect CTotalSpaceTextRect;
	SDL_FRect CFreeSpaceTextRect;
	SDL_FRect CSpaceInUseTextRect;
	SDL_FRect CPercentSpaceTextRect;

	SDL_FRect DDiscInfoTextRect;
	SDL_FRect DTotalSpaceTextRect;
	SDL_FRect DFreeSpaceTextRect;
	SDL_FRect DSpaceInUseTextRect;
	SDL_FRect DPercentSpaceTextRect;

	// cpu rects
	SDL_FRect CpuInfoTextRect;
	SDL_FRect CpuUsageTextRect;
	SDL_FRect LogicalProcessorsTextRect;

	struct CPUTime* cpuinfo;
	SYSTEM_INFO sysinfo;

	SDL_Event event;
	bool is_running;
};

bool SDLInit(struct App* a);

void AppFree(struct App** app);

bool SDLLoad(struct App* a);

void AppEvents(struct App* a);
void UpdateValues(struct App* a, struct MemInfo* meminfo, struct DiscInfo* discinfo);

void SDLRun(struct App* a);

