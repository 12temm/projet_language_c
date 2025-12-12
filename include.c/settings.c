#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "settings.h"
#include "button.h"

// Crée le bouton paramètres (utilisé dans le main)
Button createSettingsButton(int x, int y) {
    Button b;
    b.rect = (SDL_Rect){x, y, 200, 60};
    b.color = (SDL_Color){80, 80, 255, 255};
    b.label = "Parametres";
    b.visible = 1;
    return b;
}

// Calcule le volume (0-128) selon où on clique sur la barre
int getVolumeFromClick(int mx, int my) {
    SDL_Rect bar = {300, 200, 200, 30};
    if (mx >= bar.x && mx <= bar.x + bar.w && my >= bar.y && my <= bar.y + bar.h) {
        float ratio = (float)(mx - bar.x) / (float)bar.w;
        return (int)(ratio * 128);
    }
    return -1;
}

// Dessine le menu Audio (Barre de volume + Bouton retour)
void drawSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, int mx, int my, Button btnRet, int volume) {
    SDL_Color white = {255, 255, 255};

    SDL_Surface* sVol = TTF_RenderText_Blended(font, "Volume Musique", white);
    SDL_Texture* tVol = SDL_CreateTextureFromSurface(renderer, sVol);
    SDL_Rect rVol = {300, 150, 200, 40};
    SDL_RenderCopy(renderer, tVol, NULL, &rVol);
    SDL_FreeSurface(sVol);
    SDL_DestroyTexture(tVol);

    // Fond de la barre (Gris)
    SDL_Rect barBG = {300, 200, 200, 30};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &barBG);

    // Niveau du volume (Vert)
    float ratio = (float)volume / 128.0f;
    SDL_Rect barFill = {300, 200, (int)(200 * ratio), 30};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &barFill);

    // Bouton Retour
    SDL_Surface* sRet = TTF_RenderText_Blended(font, btnRet.label, white);
    SDL_Texture* tRet = SDL_CreateTextureFromSurface(renderer, sRet);

    SDL_Color hover = {180, 180, 180, 255};
    drawColoredButton(renderer, btnRet, tRet, mx, my, hover);

    SDL_FreeSurface(sRet);
    SDL_DestroyTexture(tRet);
}