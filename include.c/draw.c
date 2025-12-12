#include "draw.h"

void drawPlayer(SDL_Renderer* renderer, Chara* player) {
    if (player->orientation == 1) {
        SDL_RenderCopy(renderer, player->textureR, NULL, &player->rect);
    } else {
        SDL_RenderCopy(renderer, player->textureL, NULL, &player->rect);
    }
}