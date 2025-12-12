#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char* label;
    int visible;
} Button;

#endif