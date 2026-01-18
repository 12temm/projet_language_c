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

    SDL_Point center;
    center.x = -10;
    center.y = 0;

    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    int mouseLeftPressed = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    int dx = mouseX - (player->x + player->rect.w/2);
    int dy = mouseY - (player->y + player->rect.h/2);

    SDL_Rect armeRect;
    armeRect.w = player->rect.w + 20;
    armeRect.h = player->rect.h + 10;

    SDL_Texture* currentWeaponTexture = NULL;

    if (pickedSword == 1 && mouseLeftPressed) {
        if (abs(dx) > abs(dy)) {
            if (dx > 0) {
                currentWeaponTexture = arme->textureR;
                armeRect.x = (int)player->x + player->rect.w-5;
                armeRect.y = (int)player->y+ player->rect.h/2 -armeRect.h/2 +10;
                armeRect.w = 30;
                armeRect.h = 20;
            } else {
                currentWeaponTexture = arme->textureL;
                armeRect.x = (int)player->x - 25;
                armeRect.y = (int)player->y + player->rect.h/2 - armeRect.h/2 +10;
                armeRect.w = 30;
                armeRect.h = 20;
            }
        } else {
            if (dy > 0) {
                currentWeaponTexture = arme->textureR2;
                armeRect.x = (int)player->x + player->rect.w/2 - armeRect.w/2;
                armeRect.y = (int)player->y + player->rect.h -10;
                armeRect.w = 20;
                armeRect.h = 30;
            } else {
                currentWeaponTexture = arme->textureL2;
                armeRect.x = (int)player->x - player->rect.w/2 + armeRect.w/2;
                armeRect.y = (int)player->y - 10;
                armeRect.w = 20;
                armeRect.h = 30;
            }
        }
    }

    arme->rect.x = armeRect.x;
    arme->rect.y = armeRect.y;
    arme->rect.w = armeRect.w;
    arme->rect.h = armeRect.h;


    arme->type = 0;

    if (player->orientation == 1) {
        if (player->currentFrame == 0) {
            SDL_RenderCopy(renderer, player->textureR, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
                arme->type = 1;
            }
        }
        else if (player->currentFrame == 1) {
            SDL_RenderCopy(renderer, player->textureR2, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
            }
        }
        else {
            SDL_RenderCopy(renderer, player->textureR3, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
            }
        }
    } else {
        if (player->currentFrame == 0) {
            SDL_RenderCopy(renderer, player->textureL, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
            }
        }
        else if (player->currentFrame == 1) {
            SDL_RenderCopy(renderer, player->textureL2, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
            }
        }
        else {
            SDL_RenderCopy(renderer, player->textureL3, NULL, &player->rect);
            if (pickedSword == 1 && mouseLeftPressed && currentWeaponTexture != NULL) {
                SDL_RenderCopy(renderer, currentWeaponTexture, NULL, &armeRect);
                arme->type = 1;
            }
        }
    }


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