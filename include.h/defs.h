#pragma once
#include <SDL2/SDL.h>

#define W_WINDOW 800
#define H_WINDOW 800

typedef struct {
    float x, y, vx, vy, speed;
    int orientation;
    SDL_Texture *textureR, *textureL;
    SDL_Rect rect;
} Chara;

typedef struct {
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} KeyConfig;