#include <stdio.h>
#include "game_setting.h"
#include "settings.h"
#include "draw.h"
#include "Utils.h"

void updateKeyTexture(SDL_Renderer* ren, TTF_Font* font, SDL_Texture** tex, const char* prefix, SDL_Scancode code, SDL_Color col) {
    if (*tex) SDL_DestroyTexture(*tex);

    char buffer[64];
    sprintf(buffer, "%s  %s", prefix, SDL_GetScancodeName(code));

    SDL_Surface* tmp = TTF_RenderText_Blended(font, buffer, col);
    *tex = SDL_CreateTextureFromSurface(ren, tmp);
    SDL_FreeSurface(tmp);
}

void initSettings(SDL_Renderer* renderer, TTF_Font* font, SettingsContext* ctx, KeyConfig* keys) {
    SDL_Color white = {255, 255, 255};
    SDL_Color blue = {80, 80, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};
    SDL_Surface* tmp;

    tmp = TTF_RenderText_Blended(font, "AUDIO", white);
    ctx->tAudio = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(font, "CONTROLES", white);
    ctx->tKeyb = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(font, "RETOUR", white);
    ctx->tBack = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    ctx->tKeyUp = NULL; ctx->tKeyDown = NULL; ctx->tKeyLeft = NULL; ctx->tKeyRight = NULL;
    updateKeyTexture(renderer, font, &ctx->tKeyUp,    "Avancer :", keys->up, white);
    updateKeyTexture(renderer, font, &ctx->tKeyDown,  "Reculer :", keys->down, white);
    updateKeyTexture(renderer, font, &ctx->tKeyLeft,  "Gauche  :", keys->left, white);
    updateKeyTexture(renderer, font, &ctx->tKeyRight, "Droite  :", keys->right, white);

    int btnW = 400;
    int btnH = 50;
    int centerX = (W_WINDOW - btnW) / 2;
    int startY = 200;
    int gap = 60;

    ctx->rKeyUp    = (SDL_Rect){centerX, startY, btnW, btnH};
    ctx->rKeyDown  = (SDL_Rect){centerX, startY + gap, btnW, btnH};
    ctx->rKeyLeft  = (SDL_Rect){centerX, startY + gap*2, btnW, btnH};
    ctx->rKeyRight = (SDL_Rect){centerX, startY + gap*3, btnW, btnH};

    int navW = 200;
    int navH = 60;
    int navX = (W_WINDOW - navW) / 2;

    ctx->btnToAudio = (Button){{navX, 200, navW, navH}, blue, "Audio", 1};
    ctx->btnToKeys = (Button){{navX, 300, navW, navH}, blue, "Clavier", 1};
    ctx->btnRetGlobal = (Button){{navX, 500, navW, navH}, gray, "Retour", 1};

    ctx->currentPage = 0;
    ctx->fromGame = 0;
    ctx->rebindingId = -1;
}

void handleSettingsEvents(SettingsContext* ctx, int mx, int my, int* inSettings, int* inMenu, int* inGame, int* isPaused, int* volume) {
    if (ctx->rebindingId != -1) return;

    if (ctx->currentPage == 0) {
        if (isMouseInside(ctx->btnToAudio.rect, mx, my)) { ctx->currentPage = 1; }
        else if (isMouseInside(ctx->btnToKeys.rect, mx, my)) { ctx->currentPage = 2; }
        else if (isMouseInside(ctx->btnRetGlobal.rect, mx, my)) {
            *inSettings = 0;
            if (ctx->fromGame == 1) { *inGame = 1; *isPaused = 1; }
            else { *inMenu = 1; }
            ctx->currentPage = 0;
        }
    }
    else if (ctx->currentPage == 1) {
        if (isMouseInside(ctx->btnRetGlobal.rect, mx, my)) { ctx->currentPage = 0; }
        int nv = getVolumeFromClick(mx, my);
        if (nv != -1) { *volume = nv; Mix_VolumeMusic(*volume); }
    }
    else if (ctx->currentPage == 2) {
        if (isMouseInside(ctx->btnRetGlobal.rect, mx, my)) { ctx->currentPage = 0; }

        if (isMouseInside(ctx->rKeyUp, mx, my))    ctx->rebindingId = 0;
        else if (isMouseInside(ctx->rKeyDown, mx, my))  ctx->rebindingId = 1;
        else if (isMouseInside(ctx->rKeyLeft, mx, my))  ctx->rebindingId = 2;
        else if (isMouseInside(ctx->rKeyRight, mx, my)) ctx->rebindingId = 3;
    }
}

void handleRebind(SettingsContext* ctx, SDL_Scancode key, KeyConfig* keys, SDL_Renderer* ren, TTF_Font* font) {
    if (ctx->rebindingId == -1) return;

    if (key == SDL_SCANCODE_ESCAPE) {
        ctx->rebindingId = -1;
        return;
    }

    SDL_Color white = {255, 255, 255};

    if (ctx->rebindingId == 0) {
        keys->up = key;
        updateKeyTexture(ren, font, &ctx->tKeyUp, "Avancer :", key, white);
    }
    else if (ctx->rebindingId == 1) {
        keys->down = key;
        updateKeyTexture(ren, font, &ctx->tKeyDown, "Reculer :", key, white);
    }
    else if (ctx->rebindingId == 2) {
        keys->left = key;
        updateKeyTexture(ren, font, &ctx->tKeyLeft, "Gauche  :", key, white);
    }
    else if (ctx->rebindingId == 3) {
        keys->right = key;
        updateKeyTexture(ren, font, &ctx->tKeyRight, "Droite  :", key, white);
    }

    ctx->rebindingId = -1;
}

void drawSettings(SDL_Renderer* renderer, SettingsContext* ctx, TTF_Font* font, int volume) {
    SDL_Color blue = {80, 80, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (ctx->currentPage == 0) {
        drawColoredButton(renderer, ctx->btnToAudio, ctx->tAudio, mx, my, blue);
        drawColoredButton(renderer, ctx->btnToKeys, ctx->tKeyb, mx, my, blue);
        drawColoredButton(renderer, ctx->btnRetGlobal, ctx->tBack, mx, my, gray);
    }
    else if (ctx->currentPage == 1) {
        drawSettingsMenu(renderer, font, mx, my, ctx->btnRetGlobal, volume);
    }
    else if (ctx->currentPage == 2) {
        SDL_Rect* rects[4] = {&ctx->rKeyUp, &ctx->rKeyDown, &ctx->rKeyLeft, &ctx->rKeyRight};
        SDL_Texture* texs[4] = {ctx->tKeyUp, ctx->tKeyDown, ctx->tKeyLeft, ctx->tKeyRight};

        for(int i = 0; i < 4; i++) {
            SDL_Rect r = *rects[i];

            SDL_Color bgColor = {40, 40, 40, 255};

            if (ctx->rebindingId == i) {
                bgColor = (SDL_Color){200, 100, 0, 255};
            } else if (isMouseInside(r, mx, my) && ctx->rebindingId == -1) {
                bgColor = (SDL_Color){80, 80, 80, 255};
            }

            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &r);

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(renderer, &r);

            int w, h;
            SDL_QueryTexture(texs[i], NULL, NULL, &w, &h);
            SDL_Rect txtRect = { r.x + (r.w - w)/2, r.y + (r.h - h)/2, w, h };
            SDL_RenderCopy(renderer, texs[i], NULL, &txtRect);
        }

        if (ctx->rebindingId != -1) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
            SDL_Rect overlay = {0, 0, W_WINDOW, 100};
            SDL_RenderFillRect(renderer, &overlay);

            SDL_Color warnCol = {255, 255, 0, 255};
            SDL_Surface* sWarn = TTF_RenderText_Blended(font, "Appuyez sur une touche...", warnCol);
            SDL_Texture* tWarn = SDL_CreateTextureFromSurface(renderer, sWarn);
            SDL_Rect rWarn = {(W_WINDOW - sWarn->w)/2, 30, sWarn->w, sWarn->h};
            SDL_RenderCopy(renderer, tWarn, NULL, &rWarn);
            SDL_FreeSurface(sWarn);
            SDL_DestroyTexture(tWarn);
        }

        drawColoredButton(renderer, ctx->btnRetGlobal, ctx->tBack, mx, my, gray);
    }
}

void cleanSettings(SettingsContext* ctx) {
    SDL_DestroyTexture(ctx->tAudio);
    SDL_DestroyTexture(ctx->tKeyb);
    SDL_DestroyTexture(ctx->tBack);
    SDL_DestroyTexture(ctx->tKeyUp);
    SDL_DestroyTexture(ctx->tKeyDown);
    SDL_DestroyTexture(ctx->tKeyLeft);
    SDL_DestroyTexture(ctx->tKeyRight);
}