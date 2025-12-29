#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct Track { char *path; char *title; Mix_Music *music; } Track;

static Track *tracks = NULL;
static int track_count = 0;
static int current_index = -1;
static int is_paused = 0;
static float current_volume = 1.0f;

static char *strdup_safe(const char *s){ if(!s) return NULL; char *r = strdup(s); return r; }

static int has_ext_ogg(const char *name){
    const char *p = strrchr(name, '.');
    if (!p) return 0;
    const char *ext = p;
    // case-insensitive compare
    if (strlen(ext) != 4) return 0;
    char e0 = ext[1]; char e1 = ext[2]; char e2 = ext[3];
    if ((e0=='o' || e0=='O') && (e1=='g' || e1=='G') && (e2=='g' || e2=='G')) return 1;
    return 0;
}

int audio_init_dir(const char *musicDir)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
        return 0;
    }

    DIR *d = opendir(musicDir);
    if (!d) {
        fprintf(stderr, "audio_init_dir: could not open dir '%s'\n", musicDir);
        return 0;
    }

    struct dirent *ent;
    char fullpath[2048];
    struct stat st;

    while ((ent = readdir(d)) != NULL) {
        // skip . and ..
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        if (!has_ext_ogg(ent->d_name)) continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", musicDir, ent->d_name);
        if (stat(fullpath, &st) != 0) continue;
        if (!S_ISREG(st.st_mode)) continue; // ensure it's a regular file
        Mix_Music *m = Mix_LoadMUS(fullpath);
            if (!m) {
                fprintf(stderr, "audio_init_dir: Mix_LoadMUS('%s') failed: %s\n", fullpath, Mix_GetError());
                continue;
            }
            tracks = (Track*)realloc(tracks, sizeof(Track)*(track_count+1));
            tracks[track_count].path = strdup_safe(fullpath);
            // title = filename without extension
            char *dot = strrchr(ent->d_name, '.');
            int titleLen = dot ? (int)(dot - ent->d_name) : (int)strlen(ent->d_name);
            tracks[track_count].title = (char*)malloc(titleLen+1);
            strncpy(tracks[track_count].title, ent->d_name, titleLen);
            tracks[track_count].title[titleLen] = '\0';
            tracks[track_count].music = m;
            track_count++;
        }
    closedir(d);

    if(track_count > 0) {
        current_index = 0;
        Mix_VolumeMusic((int)(MIX_MAX_VOLUME * current_volume));
        Mix_PlayMusic(tracks[current_index].music, -1);
        is_paused = 0;
        return 1;
    }
    return 0;
}

void audio_quit(void)
{
    if (tracks) {
        for (int i = 0; i < track_count; i++) {
            if (tracks[i].music) Mix_FreeMusic(tracks[i].music);
            if (tracks[i].path) free(tracks[i].path);
            if (tracks[i].title) free(tracks[i].title);
        }
        free(tracks); tracks = NULL; track_count = 0; current_index = -1;
    }
    Mix_HaltMusic();
    Mix_CloseAudio();
}

void audio_play(int index)
{
    if (track_count == 0) return;
    if (index < 0) index = 0;
    if (index >= track_count) index = track_count-1;
    if (index == current_index) {
        if (is_paused) { Mix_ResumeMusic(); is_paused = 0; }
        return;
    }
    Mix_HaltMusic();
    current_index = index;
    Mix_PlayMusic(tracks[current_index].music, -1);
    Mix_VolumeMusic((int)(MIX_MAX_VOLUME * current_volume));
    is_paused = 0;
}

void audio_pause_toggle(void)
{
    if (track_count == 0) return;
    if (is_paused) { Mix_ResumeMusic(); is_paused = 0; }
    else { Mix_PauseMusic(); is_paused = 1; }
}

void audio_next(void){ if(track_count==0) return; audio_play((current_index+1)%track_count); }
void audio_prev(void){ if(track_count==0) return; audio_play((current_index-1+track_count)%track_count); }
void audio_set_volume(float v){ if (v<0) v=0; if (v>1) v=1; current_volume=v; Mix_VolumeMusic((int)(MIX_MAX_VOLUME*current_volume)); }

int audio_get_track_count(void){ return track_count; }
const char *audio_get_current_title(void){ if (track_count==0 || current_index<0) return NULL; return tracks[current_index].title; }
const char *audio_get_title_at(int index){ if (index<0 || index>=track_count) return NULL; return tracks[index].title; }
int audio_select(int index){ if (index<0 || index>=track_count) return 0; audio_play(index); return 1; }
