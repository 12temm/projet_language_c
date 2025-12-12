#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>

#define W_WINDOW 800
#define H_WINDOW 800

typedef struct {
    float x, y;
    float vx, vy;
    float speed;
    int orientation;
    SDL_Texture* textureR;
    SDL_Texture* textureL;
    SDL_Rect rect;
} Chara;

#endif