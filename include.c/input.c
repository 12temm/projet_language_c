#include "input.h"

void updatePlayer(Chara* player, const Uint8* state) {
    if (state[SDL_SCANCODE_A]) { player->vx = -player->speed; player->orientation = 2; }
    if (state[SDL_SCANCODE_D]) { player->vx =  player->speed; player->orientation = 1; }
    if (state[SDL_SCANCODE_W]) player->vy = -player->speed;
    if (state[SDL_SCANCODE_S]) player->vy =  player->speed;

    player->x += player->vx;
    player->y += player->vy;

    double bx = W_WINDOW * 0.037, by = H_WINDOW * 0.026;
    if (player->x <= bx) player->x = bx + 1;
    if (player->x >= W_WINDOW - bx - player->rect.w) player->x = W_WINDOW - bx - player->rect.w - 1;
    if (player->y <= by) player->y = by + 1;
    if (player->y >= H_WINDOW - by - player->rect.h) player->y = H_WINDOW - by - player->rect.h - 1;

    player->rect.x = (int)player->x;
    player->rect.y = (int)player->y;
}