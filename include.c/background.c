#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

int surface_background_check(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Surface* background) {
    if (background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}
int texture_background_check(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Texture *texture_background) {
    if (texture_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

}

int show_background(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Texture *texture_background, SDL_Rect rect) {
    if (SDL_QueryTexture(texture_background,NULL,NULL, &rect.w, &rect.h) != 0) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    rect.x = (800 -rect.w)/2;
    rect.y = (800 -rect.h)/2;

    SDL_RenderCopy(renderer, texture_background, NULL, &rect);
}


void destroy_background(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Surface* background = IMG_Load("assets/images/background_game.png");
    if (background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_background = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);
    if(texture_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Surface* object_background = IMG_Load("assets/images/object.png");
    if (object_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_object_background = SDL_CreateTextureFromSurface(renderer, object_background);
    SDL_FreeSurface(object_background);
    if(texture_object_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_DestroyTexture(texture_background);
    SDL_DestroyTexture(texture_object_background);
}