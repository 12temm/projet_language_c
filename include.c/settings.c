#include "settings.h"
#include "Utils.h"
#include <stdio.h>

#define BAR_X 300
#define BAR_Y 250
#define BAR_W 200
#define BAR_H 30

Button createSettingsButton(int x, int y) {
    Button btn;
    btn.rect = (SDL_Rect){x, y, 200, 60};
    btn.color = (SDL_Color){0, 0, 200, 255};
    btn.label = "Parametres";
    btn.visible = 1;
    return btn;
}

void drawSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, int mx, int my, Button btnReturn, int currentVolume) {
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Surface* sTitle = TTF_RenderText_Blended(font, "--- REGLAGES ---", white);
    SDL_Texture* tTitle = SDL_CreateTextureFromSurface(renderer, sTitle);
    SDL_Rect rTitle = {(800 - sTitle->w)/2, 50, sTitle->w, sTitle->h};
    SDL_RenderCopy(renderer, tTitle, NULL, &rTitle);
    SDL_FreeSurface(sTitle); SDL_DestroyTexture(tTitle);

    char volText[32];
    sprintf(volText, "Volume : %d %%", (int)((currentVolume / 128.0) * 100));
    SDL_Surface* sVol = TTF_RenderText_Blended(font, volText, white);
    SDL_Texture* tVol = SDL_CreateTextureFromSurface(renderer, sVol);
    SDL_Rect rVol = {BAR_X, BAR_Y - 40, sVol->w, sVol->h};
    SDL_RenderCopy(renderer, tVol, NULL, &rVol);
    SDL_FreeSurface(sVol); SDL_DestroyTexture(tVol);

    SDL_Rect bgBar = {BAR_X, BAR_Y, BAR_W, BAR_H};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bgBar);

    int filledW = (int)((currentVolume / 128.0) * BAR_W);
    SDL_Rect fillBar = {BAR_X, BAR_Y, filledW, BAR_H};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &fillBar);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgBar);

    SDL_Surface* sRet = TTF_RenderText_Blended(font, btnReturn.label, white);
    SDL_Texture* tRet = SDL_CreateTextureFromSurface(renderer, sRet);
    drawColoredButton(renderer, btnReturn, tRet, mx, my, (SDL_Color){200, 50, 50, 255});
    SDL_FreeSurface(sRet); SDL_DestroyTexture(tRet);
}

int getVolumeFromClick(int mx, int my) {
    SDL_Rect barRect = {BAR_X, BAR_Y, BAR_W, BAR_H};
    if (isMouseInside(barRect, mx, my)) {
        float relativeX = mx - BAR_X;
        int newVol = (int)((relativeX / BAR_W) * 128);
        if (newVol < 0) newVol = 0;
        if (newVol > 128) newVol = 128;
        return newVol;
    }
    return -1;
}