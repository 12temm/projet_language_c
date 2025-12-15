#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#include "structure.h"
#include "init_global.h"

int isMouseInside_weapons(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

int choose_weapon(SDL_Window* window, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer, int mx, int my){
    SDL_Surface* surfaceCouteau = IMG_Load("assets/images/couteau.png");
    SDL_Texture* textureCouteau = SDL_CreateTextureFromSurface(renderer, surfaceCouteau);
    SDL_FreeSurface(surfaceCouteau);
    SDL_Surface* surfaceCouteauHover = IMG_Load("assets/images/couteauhover.png");
    SDL_Texture* textureCouteauHover = SDL_CreateTextureFromSurface(renderer, surfaceCouteauHover);
    SDL_FreeSurface(surfaceCouteauHover);

    SDL_Surface* surfacePistolet = IMG_Load("assets/images/pistolet.png");
    SDL_Texture* texturePistolet = SDL_CreateTextureFromSurface(renderer, surfacePistolet);
    SDL_FreeSurface(surfacePistolet);
    SDL_Surface* surfacePistoletHover = IMG_Load("assets/images/pistolethover.png");
    SDL_Texture* texturePistoletHover = SDL_CreateTextureFromSurface(renderer, surfacePistoletHover);
    SDL_FreeSurface(surfacePistoletHover);

    Button swordButton ={{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword",1};
    Button gunButton ={{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun",1};
    if (isMouseInside_weapons(swordButton.rect, mx, my)) {
        SDL_RenderCopy(renderer, textureCouteauHover, NULL, &swordButton.rect);
    } else {
        SDL_RenderCopy(renderer, textureCouteau, NULL, &swordButton.rect);
    }
    if (isMouseInside_weapons(gunButton.rect, mx, my)) {
        SDL_RenderCopy(renderer, texturePistoletHover, NULL, &gunButton.rect);
    } else {
        SDL_RenderCopy(renderer, texturePistolet, NULL, &gunButton.rect);
    }
}

void destroy_menu_weapons(SDL_Window* window, TTF_Font* font, SDL_Color color) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Noob 32", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Surface* surfaceCouteau = IMG_Load("assets/images/couteau.png");
    SDL_Texture* textureCouteau = SDL_CreateTextureFromSurface(renderer, surfaceCouteau);
    SDL_FreeSurface(surfaceCouteau);

    SDL_Surface* surfaceCouteauHover = IMG_Load("assets/images/couteauhover.png");
    SDL_Texture* textureCouteauHover = SDL_CreateTextureFromSurface(renderer, surfaceCouteauHover);
    SDL_FreeSurface(surfaceCouteauHover);

    SDL_Surface* surfacePistolet = IMG_Load("assets/images/pistolet.png");
    SDL_Texture* texturePistolet = SDL_CreateTextureFromSurface(renderer, surfacePistolet);
    SDL_FreeSurface(surfacePistolet);

    SDL_Surface* surfacePistoletHover = IMG_Load("assets/images/pistolethover.png");
    SDL_Texture* texturePistoletHover = SDL_CreateTextureFromSurface(renderer, surfacePistoletHover);

    SDL_DestroyTexture(textureCouteau);
    SDL_DestroyTexture(textureCouteauHover);
    SDL_DestroyTexture(texturePistolet);
    SDL_DestroyTexture(texturePistoletHover);
}