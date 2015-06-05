#include "SDLEventHandler.h"

#include <android/log.h>
#define  LOG_TAG    __FILE__
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


extern int CurrentDisplayGOP;
extern bool quit;

extern CRITICAL_SECTION CurrentDisplayMutex;
extern CONDITION_VARIABLE CurrentDisplayCond;
// extern sem_t frameStoreSem;

SDLEventHandler::SDLEventHandler (MyMonitor *m) :
		win (NULL),
		renderer (NULL),
		bmp (NULL),
		win_width (0),
		win_height (0),
		displayedFrames (0),
		monitor (m),
		down_x (0.0),
		down_y (0.0),
		timestampe (0)
{
	InitializeCriticalSection(&this->quit_mutex);

	if (!this->Init()) {
		LOGE("SDL Init failed : %s", SDL_GetError());
		this->SetQuit(true);
	}
	else {
		LOGI("SDL Initialization");
	}
}
SDLEventHandler::~SDLEventHandler () {
	this->Release();
	// delete monitor;
}

// Public Functions
void SDLEventHandler::Release () {
	SDL_DestroyTexture(this->bmp);
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->win);
}
bool SDLEventHandler::IsFinished () {
	EnterCriticalSection(&quit_mutex);
	bool reutrn_quit = quit;
	LeaveCriticalSection(&quit_mutex);

	return reutrn_quit;
}
void SDLEventHandler::StartLoop () {
	bool startDisplay = false;

	while (!this->IsFinished()) {
		SDL_Event e;
		// Event Polling
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					this->SetQuit(true);
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_AC_BACK)
						this->SetQuit(true);
					break;
				// case SDL_FINGERMOTION:
				// 	this->GestureEvent(e);
				// 	break;
				case SDL_FINGERDOWN:
					this->down_x = e.tfinger.x;
					this->down_y = e.tfinger.y;
					break;
				case SDL_FINGERUP:
					this->SwitchEvent(e);
					break;

				/* Custom Event */
				case DISPLAY_EVENT:
					if (!startDisplay) {
						startDisplay = true;
						monitor->SetTotalStartTime();
						monitor->SetGOPStartTime();
					}
					// LOGD("[StarLoop] - Get DISPLAY_EVENT");
					this->DisplayEvent(e);

					// LOGW("[StartLoop] - %s more DISPLAY_EVENT", SDL_HasEvent(DISPLAY_EVENT) ? "Has" : "Hasn't");
					break;
				case SKIP_FRAME_EVENT:
					LOGD("[StarLoop] - Get SKIP_FRAME_EVENT");
					this->ChangeGOPNumber(1);
					break;
				case QUIT_EVENT:
					this->SetQuit(true);
					break;
				case RESET_GOP_NUMBER:
					LOGD("[StarLoop] - Get RESET_GOP_NUMBER");
					this->ChangeGOPNumber(-1);
					break;
				default:
					break;
			}
		}
	}
}
void SDLEventHandler::SetQuit (bool set_quit) {
	EnterCriticalSection(&quit_mutex);
		quit = set_quit;
	LeaveCriticalSection(&quit_mutex);
}
MyMonitor* SDLEventHandler::GetMonitor () {
	return this->monitor;
}

// Private Functions
bool SDLEventHandler::Init () {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	this->win_width = mode.w;
	this->win_height = mode.h;
	LOGI("[Init] - Display Size is %dx%d", this->win_width, this->win_height);

	// this->win = SDL_CreateWindow(NULL, 0, 0, win_width, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	this->win = SDL_CreateWindow(NULL, 0, 0, win_width, win_height, SDL_WINDOW_OPENGL);
	if (this->win == NULL) {
		LOGE("[Init] SDL Window Created failed : %s", SDL_GetError());
		return false;
	}

	int numdrivers = SDL_GetNumRenderDrivers ();	
	LOGI("[Init] - Render driver count: %d", numdrivers);

	for (int i = 0; i < numdrivers; i++) {
		SDL_RendererInfo drinfo;
		SDL_GetRenderDriverInfo(i, &drinfo);
		LOGI("[Init] - Driver name: \"%s\": ", drinfo.name);
		if (drinfo.flags & SDL_RENDERER_SOFTWARE)
			LOGI("[Init] -   The renderer is a software fallback");
		if (drinfo.flags & SDL_RENDERER_ACCELERATED)
			LOGI("[Init] -   The renderer uses hardware acceleration");
		if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC)
			LOGI("[Init] -   Present is synchronized with the refresh rate");
		if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
			LOGI("[Init] -   The renderer supports rendering to texture");
	}

	this->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	// this->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (this->renderer == NULL) {
		LOGE("[Init] SDL Renderer Created failed : %s", SDL_GetError());
		return false;
	}
	
	this->bmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
	if (this->bmp == NULL) {
		LOGE("[Init] SDL Texture Created failed : %s", SDL_GetError());
		return false;
	}

	// this->monitor = new MyMonitor();

	return true;
}
void SDLEventHandler::GestureEvent (SDL_Event e) {
	int r = 0, g = 0, b = 0;
	r = (int) (e.tfinger.x * 255);
	g = (int) (e.tfinger.y * 255);
	b = (int) ((r+g) / 2);
	SDL_RenderClear(this->renderer);
	SDL_SetRenderDrawColor(this->renderer, r, g, b, 255);
	SDL_RenderPresent(this->renderer);
}
void SDLEventHandler::SwitchEvent (SDL_Event e) {
	float dx = e.tfinger.x - this->down_x;
	float dy = e.tfinger.y - this->down_y;

	LOGI("[SwitchEvent] - dx=%f, dy=%f", dx, dy);

	// Up = SID++, Down = SID--
	if (dy < -0.4) {
		this->monitor->HigherSID();
	}
	else if (dy > 0.4) {
		this->monitor->LowerSID();
	}

	// Right = TID++, Left = TID--
	if (dx < -0.4) {
		this->monitor->LowerTID();
	}
	else if (dx > 0.4) {
		this->monitor->HigherTID();
	}
}
void SDLEventHandler::DisplayEvent (SDL_Event e) {
	// SDLDisplayInfo *info = (SDLDisplayInfo*) e.user.data1;
	SDLFrame *info = (SDLFrame*) e.user.data1;
	// LOGI("[DisplayEvent] - Displaying Frame from frameList[%d] by Decoder[%d]", info.current_frame_index, obj.ThreadID);
	
	uint32_t startTime = SDL_GetTicks();

	// If this frame is first one in this GOP
	// if (this->displayedFrames == 0) {
	// 	monitor->SetGOPStartTime();	// Set Time
	// 	// this->timestampe = SDL_GetTicks();
	// }

	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = info->width;
	rect.h = info->height;

	this->CheckResolution(rect);

	// Reneder this Frame
	SDL_RenderClear(this->renderer);
	SDL_UpdateTexture(this->bmp, NULL, info->frame.RGB, rect.w*2);
	SDL_RenderCopy(this->renderer, this->bmp, NULL, &rect);
	SDL_RenderPresent(this->renderer);
	

	this->displayedFrames++;

	if (this->displayedFrames == info->gop_frames) {
		this->displayedFrames = 0;

		LOGV("[DisplayEvent] - GOP %d Done", info->gop_no);
		monitor->SetFrames(info->gop_frames);
		monitor->WriteToOutputFile(info->gop_no, info->gop_frames);

		
		this->ChangeGOPNumber(1);
	}

	// free(info->frame.RGB);
	// info->frame.alloc_size = 0;
	// free(info);

	monitor->SetRenderingTime(SDL_GetTicks() - startTime);
}
void SDLEventHandler::CheckResolution (SDL_Rect rect) {
	int w = 0, h = 0;
	SDL_QueryTexture(this->bmp, NULL, NULL, &w, &h);

	if (w != rect.w || h != rect.h) {
		SDL_DestroyTexture(this->bmp);
		// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		this->bmp = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_BGR565, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
		LOGI("[CheckResolution] - Reset Texture Size from %dx%d to %dx%d", w, h, rect.w, rect.h);
	}
}
void SDLEventHandler::ChangeGOPNumber (int value) {
	if ( (value != 1) && (value != -1) ) return;

	// LOGD("[IncreaseGopNo:DeadLock] - Before CS");
	EnterCriticalSection(&CurrentDisplayMutex);
	// LOGD("[IncreaseGopNo:DeadLock] - Inside CS");
		CurrentDisplayGOP += value;
		LOGV("[IncreaseGopNo] - Set GOP from %d to %d", CurrentDisplayGOP-value, CurrentDisplayGOP);
		WakeAllConditionVariable(&CurrentDisplayCond);
	LeaveCriticalSection(&CurrentDisplayMutex);
	// LOGD("[IncreaseGopNo:DeadLock] - Leave CS");
}