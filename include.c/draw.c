#include "structure.h"
#include "draw.h"
#include "utils.h"
#include "background.h"
#include <stdio.h>
#include <math.h>


void render_game_scene(SDL_Renderer* renderer, struct character* player, struct character* ennemis, struct character* arme, int numEnnemi,
                       struct projectile* balles, int numBalles,
                       SDL_Texture* texBg, SDL_Texture* texObj,
                       int pickedSword, int attackSpeed,
                       TTF_Font* font, int enemies_killed) {

    SDL_Rect rect = {0, 0, 0, 0};

    show_background(NULL, renderer, NULL, texBg, rect);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Point center;
    center.x = -30;
    center.y = 0;

    double angle = atan2(mouseY - player->y, mouseX - player->x);
    double angleDeg = angle * 180.0 / M_PI;

    SDL_Rect armeRect;
    armeRect.x = (int)player->x + 30;
    armeRect.y = (int)player->y;
    armeRect.w = player->rect.w + 10;
    armeRect.h = player->rect.h;

    SDL_Point rotatedPoints[4];
    SDL_Point centerAbs = {armeRect.x + center.x, armeRect.y + center.y};
    SDL_Point originalPoints[4] = {
        {armeRect.x, armeRect.y},
        {armeRect.x + armeRect.w, armeRect.y},
        {armeRect.x + armeRect.w, armeRect.y + armeRect.h},
        {armeRect.x, armeRect.y + armeRect.h}
    };
    for (int i = 0; i < 4; i++) {
        double translatedX = originalPoints[i].x - centerAbs.x;
        double translatedY = originalPoints[i].y - centerAbs.y;
        rotatedPoints[i].x = (int)(translatedX * cos(angle) - translatedY * sin(angle) + centerAbs.x);
        rotatedPoints[i].y = (int)(translatedX * sin(angle) + translatedY * cos(angle) + centerAbs.y);
    }

    /*Uint32 now = SDL_GetTicks() / attackSpeed;
    SDL_Rect armeRect;
    armeRect.x = (int)player->x+30;
    armeRect.y = (int)player->y;
    armeRect.w = player->rect.w+10;
    armeRect.h = player->rect.h;
    */

    /*SDL_Point rotatedPoints[4];
    double angle = now * M_PI / 180.0;
    SDL_Point centerAbs = {armeRect.x + center.x, armeRect.y + center.y};
    */

    /*SDL_Point originalPoints[4] = {
        {armeRect.x, armeRect.y},
        {armeRect.x + armeRect.w, armeRect.y},
        {armeRect.x + armeRect.w, armeRect.y + armeRect.h},
        {armeRect.x, armeRect.y + armeRect.h}
    };

    for (int i = 0; i < 4; i++) {
        double translatedX = originalPoints[i].x - centerAbs.x;
        double translatedY = originalPoints[i].y - centerAbs.y;
        rotatedPoints[i].x = (int)(translatedX * cos(angle) - translatedY * sin(angle) + centerAbs.x);
        rotatedPoints[i].y = (int)(translatedX * sin(angle) + translatedY * cos(angle) + centerAbs.y);
    }
    */



    if (player->orientation == 1) {
        if (player->currentFrame == 0) {
            SDL_RenderCopy(renderer, player->textureR, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
        else if (player->currentFrame == 1) {
            SDL_RenderCopy(renderer, player->textureR2, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
        else {
            SDL_RenderCopy(renderer, player->textureR3, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
    } else {
        if (player->currentFrame == 0) {
            SDL_RenderCopy(renderer, player->textureL, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
        else if (player->currentFrame == 1) {
            SDL_RenderCopy(renderer, player->textureL2, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
        else {
            SDL_RenderCopy(renderer, player->textureL3, NULL, &player->rect);
            if (pickedSword == 1) {
                SDL_RenderCopyEx(renderer, arme->normal_texture, NULL, &armeRect, angleDeg, &center, SDL_FLIP_NONE);
            }
        }
    }

    SDL_Rect fillRect = { armeRect.x, armeRect.y, armeRect.w, armeRect.h };
    SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRect( renderer, &fillRect );

    for (int i = 0; i < numEnnemi; i++) {
        if (ennemis[i].orientation == 1 && !ennemis[i].dead)
            SDL_RenderCopy(renderer, ennemis[i].textureR, NULL, &ennemis[i].rect);
        else if (!ennemis[i].dead)
            SDL_RenderCopy(renderer, ennemis[i].textureL, NULL, &ennemis[i].rect);
    }

    for (int i = 0; i < numBalles; i++) {
        if (balles[i].active) {
            SDL_RenderCopy(renderer, balles[i].texture, NULL, &balles[i].rect);
        }
    }

    show_background(NULL, renderer, NULL, texObj, rect);

    drawHealthBar(renderer, 20, 20, 200, 20, player->health, player->maxHealth);

    char scoreText[32];
    sprintf(scoreText, "Score: %d", enemies_killed);

    SDL_Color scoreColor = {255, 255, 255, 255};
    SDL_Surface* surfaceScore = TTF_RenderText_Blended(font, scoreText, scoreColor);

    if (surfaceScore) {
        SDL_Texture* textureScore = SDL_CreateTextureFromSurface(renderer, surfaceScore);
        SDL_Rect rectScore;
        rectScore.x = 20;
        rectScore.y = 50;
        rectScore.w = surfaceScore->w;
        rectScore.h = surfaceScore->h;

        SDL_RenderCopy(renderer, textureScore, NULL, &rectScore);

        SDL_FreeSurface(surfaceScore);
        SDL_DestroyTexture(textureScore);
    }
}