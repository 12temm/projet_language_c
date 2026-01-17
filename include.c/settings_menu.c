#include "settings_menu.h"
#include <stdio.h>

void render_settings_menu(SDL_Renderer* renderer, TTF_Font* font, int mx, int my, int* inSettings, int* inMenu) {
    SDL_Color white = {255, 255, 255, 255};
    int currentVol = Mix_VolumeMusic(-1);

    SDL_Rect btnMinus = {220, 200, 60, 60};
    SDL_Rect btnPlus  = {460, 200, 60, 60};
    SDL_Rect btnBack  = {220, 400, 300, 60};

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &btnMinus);
    SDL_RenderFillRect(renderer, &btnPlus);
    SDL_RenderFillRect(renderer, &btnBack);


    char volText[32];
    sprintf(volText, "Volume: %d%%", (currentVol * 100) / 128);
    SDL_Surface* sVol = TTF_RenderText_Blended(font, volText, white);
    SDL_Texture* tVol = SDL_CreateTextureFromSurface(renderer, sVol);
    SDL_Rect rVol = {300, 210, sVol->w, sVol->h};
    SDL_RenderCopy(renderer, tVol, NULL, &rVol);
    SDL_FreeSurface(sVol); SDL_DestroyTexture(tVol);


    SDL_Surface* sM = TTF_RenderText_Blended(font, "-", white);
    SDL_Texture* tM = SDL_CreateTextureFromSurface(renderer, sM);
    SDL_Rect rM = {240, 210, sM->w, sM->h};
    SDL_RenderCopy(renderer, tM, NULL, &rM);
    SDL_FreeSurface(sM); SDL_DestroyTexture(tM);

    SDL_Surface* sP = TTF_RenderText_Blended(font, "+", white);
    SDL_Texture* tP = SDL_CreateTextureFromSurface(renderer, sP);
    SDL_Rect rP = {480, 210, sP->w, sP->h};
    SDL_RenderCopy(renderer, tP, NULL, &rP);
    SDL_FreeSurface(sP); SDL_DestroyTexture(tP);

    SDL_Surface* sB = TTF_RenderText_Blended(font, "RETOUR", white);
    SDL_Texture* tB = SDL_CreateTextureFromSurface(renderer, sB);
    SDL_Rect rB = {320, 410, sB->w, sB->h};
    SDL_RenderCopy(renderer, tB, NULL, &rB);
    SDL_FreeSurface(sB); SDL_DestroyTexture(tB);
}