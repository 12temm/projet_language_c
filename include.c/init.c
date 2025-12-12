#include "init.h"
#include <stdio.h>
#include "defs.h"

int initAll(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("Jeu SDL2",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               W_WINDOW, H_WINDOW,
                               SDL_WINDOW_SHOWN);

    if (!*window) {
        printf("Erreur Window : %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erreur Renderer : %s\n", SDL_GetError());
        return 0;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
        return 0;
    }

    return 1;
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* m1, Mix_Music* m2) {
    if (m1) Mix_FreeMusic(m1);
    if (m2) Mix_FreeMusic(m2);
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}