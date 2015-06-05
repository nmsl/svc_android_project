#ifndef H_SDL_MY_EVENT_HANDLER_H
#define H_SDL_MY_EVENT_HANDLER_H

#include <queue>

#include "SDL.h"
#include "MultiThreadDefined.h"
#include "MyMonitor.h"
#include "FrameStore.h"

/* SDL Event */
enum {
	DISPLAY_EVENT = SDL_USEREVENT,
	SKIP_FRAME_EVENT,
	RESET_GOP_NUMBER,
	QUIT_EVENT,
};

// struct _handlerData {
// 	SDL_Window *win;
// 	SDL_Renderer *ren;
// 	SDL_Texture *text;

// 	int win_height, win_width;
// 	int displayed_frames;

// 	uint32_t timestampe;

// 	CRITICAL_SECTION quit_mutex;
// } SDLHandlerData;

// void SDLHandler_Init();
// void SDLHandler_Release();
// SDLBool SDLHandler_IsFinished();






class SDLEventHandler {
	public:
		SDLEventHandler (MyMonitor *m);
		~SDLEventHandler ();

		void Release ();
		bool IsFinished ();
		void SetQuit (bool set_quit);
		void StartLoop ();

		MyMonitor* GetMonitor ();

	private:
		bool Init ();
		void GestureEvent (SDL_Event e);
		void SwitchEvent (SDL_Event e);
		void DisplayEvent (SDL_Event e);
		void CheckResolution (SDL_Rect rect);
		void ChangeGOPNumber (int value);

		SDL_Window *win;
		SDL_Renderer *renderer;
		SDL_Texture *bmp;

		int win_height;
		int win_width;
		int displayedFrames;

		float down_x, down_y;

		uint32_t timestampe;

		CRITICAL_SECTION quit_mutex;
		MyMonitor *monitor;
};


#endif /* H_SDL_MY_EVENT_HANDLER_H */