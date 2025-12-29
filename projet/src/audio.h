#ifndef AUDIO_H
#define AUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>

// Initialize audio system by scanning a directory for .ogg files
// Returns 1 on success (at least one track), 0 on failure
int audio_init_dir(const char *musicDir);
void audio_quit(void);

// Controls
void audio_play(int index);
void audio_pause_toggle(void);
void audio_next(void);
void audio_prev(void);
void audio_set_volume(float v); // 0..1

// Info / selection
int audio_get_track_count(void);
const char *audio_get_current_title(void);
const char *audio_get_title_at(int index);
int audio_select(int index);

#endif
