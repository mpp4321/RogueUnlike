#include "SystemClasses.h"

void GraphicsContext::free_resources() {
	SDL_FreeSurface(windowSurface);
	SDL_DestroyWindow(window);
}

void GraphicsContext::initialize_graphics() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			windowSurface = SDL_GetWindowSurface(window);
		}
	}


}

void GraphicsContext::draw_image(SDL_Rect& dest, SDL_Surface* img)
{
	if (SDL_BlitSurface(img, NULL, windowSurface, &dest) == -1) {
		printf("Unsuccessful blit\n");
	}
}

void GraphicsContext::update()
{
	SDL_UpdateWindowSurface(window);
}

void GraphicsContext::clear()
{
	SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));
}
