#include "video.h"
#include "gestionSDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct {
    SDL_Texture **frames;
    int count;
    int current;
    int fps;
    int playing;
    unsigned int lastTick;
} Video;

static Video vid = {0};

static int has_image_ext(const char *name)
{
    const char *p = strrchr(name, '.');
    if (!p) return 0;
    // case-insensitive checks without strcasecmp for portability
    int len = strlen(p);
    if (len == 4) {
        // .png or .jpg or .bmp
        char a = p[1], b = p[2], c = p[3];
        if ((a=='p'||a=='P') && (b=='n'||b=='N') && (c=='g'||c=='G')) return 1;
        if ((a=='j'||a=='J') && (b=='p'||b=='P') && (c=='g'||c=='G')) return 1;
        if ((a=='b'||a=='B') && (b=='m'||b=='M') && (c=='p'||c=='P')) return 1;
    } else if (len == 5) {
        // .jpeg
        char a=p[1], b=p[2], c=p[3], d=p[4];
        if ((a=='j'||a=='J') && (b=='p'||b=='P') && (c=='e'||c=='E') && (d=='g'||d=='G')) return 1;
    }
    return 0;
}

int video_init_dir(SDL_Renderer *renderer, const char *dir, int fps)
{
    DIR *d = opendir(dir);
    if (!d) return 0;

    struct dirent *ent;
    struct stat st;
    char fullpath[2048];

    vid.frames = NULL;
    vid.count = 0;
    vid.current = 0;
    vid.fps = fps > 0 ? fps : 24;
    vid.playing = 0;
    vid.lastTick = 0;

    // Simple: collect names into array, sort alphabetically
    char **names = NULL;
    int namesCount = 0;

    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        if (!has_image_ext(ent->d_name)) continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, ent->d_name);
        if (stat(fullpath, &st) != 0) continue;
        if (!S_ISREG(st.st_mode)) continue;
        names = (char**)realloc(names, sizeof(char*)*(namesCount+1));
        names[namesCount] = strdup(fullpath);
        namesCount++;
    }
    closedir(d);

    if (namesCount == 0) return 0;

    // sort names
    for (int i = 0; i < namesCount - 1; i++) {
        for (int j = i+1; j < namesCount; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char *tmp = names[i]; names[i] = names[j]; names[j] = tmp;
            }
        }
    }

    for (int i = 0; i < namesCount; i++) {
        SDL_Texture *t = getTextureFromImage(names[i], renderer);
        if (t) {
            vid.frames = (SDL_Texture**)realloc(vid.frames, sizeof(SDL_Texture*)*(vid.count+1));
            vid.frames[vid.count++] = t;
        }
        free(names[i]);
    }
    free(names);

    return vid.count;
}

void video_quit(void)
{
    if (vid.frames) {
        for (int i = 0; i < vid.count; i++) SDL_DestroyTexture(vid.frames[i]);
        free(vid.frames); vid.frames = NULL; vid.count = 0;
    }
    vid.playing = 0;
}

void video_play(void)
{
    if (vid.count == 0) return;
    vid.current = 0;
    vid.playing = 1;
    vid.lastTick = SDL_GetTicks();
}

void video_stop(void)
{
    vid.playing = 0;
}

int video_is_playing(void){ return vid.playing; }

void video_update_and_render(SDL_Renderer *renderer, SDL_Rect dest)
{
    if (!vid.playing || vid.count == 0) return;
    unsigned int now = SDL_GetTicks();
    unsigned int frameMs = 1000 / vid.fps;
    if (now - vid.lastTick >= frameMs) {
        vid.current++;
        vid.lastTick = now;
        if (vid.current >= vid.count) {
            // stop at end
            vid.playing = 0;
            return;
        }
    }
    SDL_Texture *tex = vid.frames[vid.current];
    if (tex) SDL_RenderCopy(renderer, tex, NULL, &dest);
}
