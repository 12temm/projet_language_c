#include "structure.h"
#include "victory.h"
#include <stdio.h>

static int isMouseInsideVictory(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

void render_victory_screen(SDL_Renderer* renderer, TTF_Font* font, int w_window, int h_window, int mx, int my, Button* replayBtn, Button* menuBtn, Button* scoreBtn) {
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
    SDL_Rect bg = {0, 0, w_window, h_window};
    SDL_RenderFillRect(renderer, &bg);

    SDL_Color gold = {255, 215, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "VICTOIRE !", gold);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rect = {(w_window - surf->w * 2)/2, 100, surf->w * 2, surf->h * 2}; // Un peu plus haut
    SDL_RenderCopy(renderer, tex, NULL, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    if (isMouseInsideVictory(replayBtn->rect, mx, my)) SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    else SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
    SDL_RenderFillRect(renderer, &replayBtn->rect);

    SDL_Color white = {255, 255, 255, 255};
    surf = TTF_RenderText_Blended(font, replayBtn->label, white);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rectReplay = {replayBtn->rect.x + (replayBtn->rect.w - surf->w)/2, replayBtn->rect.y + (replayBtn->rect.h - surf->h)/2, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &rectReplay);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    if (isMouseInsideVictory(menuBtn->rect, mx, my)) SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    else SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
    SDL_RenderFillRect(renderer, &menuBtn->rect);

    surf = TTF_RenderText_Blended(font, menuBtn->label, white);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rectMenu = {menuBtn->rect.x + (menuBtn->rect.w - surf->w)/2, menuBtn->rect.y + (menuBtn->rect.h - surf->h)/2, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &rectMenu);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    if (isMouseInsideVictory(scoreBtn->rect, mx, my)) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    else SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
    SDL_RenderFillRect(renderer, &scoreBtn->rect);

    surf = TTF_RenderText_Blended(font, scoreBtn->label, white);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect rectScore = {scoreBtn->rect.x + (scoreBtn->rect.w - surf->w)/2, scoreBtn->rect.y + (scoreBtn->rect.h - surf->h)/2, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &rectScore);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}