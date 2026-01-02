#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>

// Initialize video player by loading all image frames from a directory (png/jpg/bmp)
// Returns number of frames loaded.
int video_init_dir(SDL_Renderer *renderer, const char *dir, int fps);
void video_quit(void);

// Start playback from beginning
void video_play(void);
void video_stop(void);
int video_is_playing(void);

// Update internal timer and render current frame into dest rect (if playing)
void video_update_and_render(SDL_Renderer *renderer, SDL_Rect dest);

#endif
