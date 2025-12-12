#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

int initAll(SDL_Window** window, SDL_Renderer** renderer);
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* m1, Mix_Music* m2);