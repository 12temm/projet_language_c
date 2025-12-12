#include "init.h"
#include "defs.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

int initAll(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 0;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        return 0;
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("SDL2 Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WINDOW, H_WINDOW, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0); // 0 ou SDL_RENDERER_ACCELERATED
    if (!*renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 0;
    }

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