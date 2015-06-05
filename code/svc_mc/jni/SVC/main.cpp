// #include <stdio.h>
// #include <stdlib.h>
// #include <android/log.h>

// #include "SDL.h"
// #include "SDL_log.h"


// #define LOG_TAG "main.cpp"
// #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
// #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

// int main (int argc, char* argv[]) {
// 	SDL_Window *win;
// 	SDL_Renderer *renderer;

// 	// Initialize SDL
// 	if (SDL_Init(SDL_INIT_VIDEO) < 0)
// 		return 1;

// 	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

// 	SDL_DisplayMode mode;
// 	SDL_GetDisplayMode(0, 0, &mode);
// 	int width = mode.w;
// 	int height = mode.h;

// 	SDL_Log("Width = %d, Height = %d. \n", width, height);

// 	// Create the window where we will draw
// 	win = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

// 	// We must call SDL_CreateRendere in order for draw calls to affect
// 	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

// 	SDL_Event e;
// 	bool quit = false;
// 	int r = 0, g = 0, b = 0;
// 	while (!quit) {
// 		// Event Polling
// 		while (SDL_PollEvent(&e)) {

// 			switch (e.type) {
// 				case SDL_QUIT:
// 					quit = true;
// 					break;
// 				case SDL_KEYDOWN:
// 					if (e.key.keysym.sym == SDLK_AC_BACK)
// 						quit = true;
// 					break;
// 				case SDL_FINGERMOTION:
// 					r = (int)(e.tfinger.x * 255);
// 					g = (int)(e.tfinger.y * 255);
// 					b = (r+g) / 2;
// 					break;
// 				default:
// 					break;
// 			}
// 		}

// 		// Rendering
// 		SDL_RenderClear(renderer);
// 		// Draw the image
// 		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
// 		// Update the screen
// 		SDL_RenderPresent(renderer);
// 	}

// 	// ALways be sure to clean up
// 	SDL_DestroyRenderer(renderer);
// 	SDL_DestroyWindow(win);
// 	SDL_Quit();

// 	// return 0;
// }