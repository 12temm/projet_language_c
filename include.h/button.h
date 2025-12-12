#pragma once
#include <SDL2/SDL.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char* label;
    int visible;
} Button;

void drawColoredButton(SDL_Renderer* renderer, Button btn, SDL_Texture* textTex, int mx, int my, SDL_Color hoverCol);
void drawImageButton(SDL_Renderer* renderer, SDL_Rect rect, SDL_Texture* normal, SDL_Texture* hover, int mx, int my);