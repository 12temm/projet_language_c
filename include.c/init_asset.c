#include "structure.h"
#include "init_assets.h"
#include <SDL2/SDL_image.h>

void init_player_assets(SDL_Renderer* renderer, Char* player) {
    player->health = 3;
    player->moving = 0;
    player->x = 300;
    player->y = 300;
    player->vx = 0;
    player->vy = 0;
    player->speed = 2;
    player->rect.w = 20;
    player->rect.h = 20;
    player->orientation = 1;
    player->currentFrame = 0;
    player->lastFrameTime = 0;
    player->frameDelay = 100;
    player->maxHealth = 3;
    player->invincible = 0;
    player->invincibleStart = 0;
    player->invincibleTime = 1000;

    SDL_Surface* bonhomme_right = IMG_Load("assets/images/bonhommeR.png");
    player->textureR = SDL_CreateTextureFromSurface(renderer, bonhomme_right);
    SDL_FreeSurface(bonhomme_right);

    SDL_Surface* bonhomme_left = IMG_Load("assets/images/bonhommeL.png");
    player->textureL = SDL_CreateTextureFromSurface(renderer, bonhomme_left);
    SDL_FreeSurface(bonhomme_left);

    SDL_Surface* bonhomme_right2 = IMG_Load("assets/images/bonhomme2R.png");
    player->textureR2 = SDL_CreateTextureFromSurface(renderer, bonhomme_right2);
    SDL_FreeSurface(bonhomme_right2);

    SDL_Surface* bonhomme_left2 = IMG_Load("assets/images/bonhomme2L.png");
    player->textureL2 = SDL_CreateTextureFromSurface(renderer, bonhomme_left2);
    SDL_FreeSurface(bonhomme_left2);

    SDL_Surface* bonhomme_right3 = IMG_Load("assets/images/bonhomme3R.png");
    player->textureR3 = SDL_CreateTextureFromSurface(renderer, bonhomme_right3);
    SDL_FreeSurface(bonhomme_right3);

    SDL_Surface* bonhomme_left3 = IMG_Load("assets/images/bonhomme3L.png");
    player->textureL3 = SDL_CreateTextureFromSurface(renderer, bonhomme_left3);
    SDL_FreeSurface(bonhomme_left3);
}

void init_enemy_assets(SDL_Renderer* renderer, Char* ennemis, int numEnnemi) {
    SDL_Surface* sNormal = IMG_Load("assets/images/mechantR.png");
    SDL_Texture* tNormal = SDL_CreateTextureFromSurface(renderer, sNormal);
    SDL_FreeSurface(sNormal);

    SDL_Surface* sTank = IMG_Load("assets/images/mechant_tank.png");
    SDL_Texture* tTank = SDL_CreateTextureFromSurface(renderer, sTank);
    SDL_FreeSurface(sTank);

    for (int i = 0; i < numEnnemi; i++) {
        ennemis[i].dead = 0;

        if (i % 5 == 0) {
            ennemis[i].type = 1;
            ennemis[i].textureR = tTank;
            ennemis[i].textureL = tTank;
        }
        else {
            ennemis[i].type = 0;
            ennemis[i].textureR = tNormal;
            ennemis[i].textureL = tNormal;
        }
    }
}


void init_weapon_assets(SDL_Renderer* renderer, Char* arme, Char* player) {
    arme->x = player->x + 50;
    arme->y = player->y + 50;
    arme->rect.w = player->rect.w;
    arme->rect.h = player->rect.h;

    SDL_Surface* armeS = IMG_Load("assets/images/spr_wep_brnz_sword_1.png");
    arme->normal_texture = SDL_CreateTextureFromSurface(renderer, armeS);
    SDL_FreeSurface(armeS);
}