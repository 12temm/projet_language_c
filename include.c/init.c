#include "init.h"
#include "defs.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

int initAll(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) return 0;
    if (IMG_Init(IMG_INIT_PNG) == 0) return 0;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;
    if (TTF_Init() == -1) return 0;

    *window = SDL_CreateWindow("SDL2 Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WINDOW, H_WINDOW, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!*window) return 0;

    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (!*renderer) return 0;

    SDL_RenderSetLogicalSize(*renderer, W_WINDOW, H_WINDOW);
    return 1;
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* m1, Mix_Music* m2) {
    if (m1) Mix_FreeMusic(m1);
    if (m2) Mix_FreeMusic(m2);
    Mix_CloseAudio();
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}