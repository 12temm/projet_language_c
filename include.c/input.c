#include "input.h"

void updatePlayer(Chara* player, const Uint8* state) {
    if (state[SDL_SCANCODE_A]) { player->vx = -player->speed; player->orientation = 2; }
    if (state[SDL_SCANCODE_D]) { player->vx =  player->speed; player->orientation = 1; }
    if (state[SDL_SCANCODE_W]) player->vy = -player->speed;
    if (state[SDL_SCANCODE_S]) player->vy =  player->speed;

    player->x += player->vx;
    player->y += player->vy;

    double border_x = W_WINDOW * 0.037;
    double border_y = H_WINDOW * 0.026;

    if (player->x <= border_x) player->x = border_x + 1;
    if (player->x >= W_WINDOW - border_x - player->rect.w) player->x = W_WINDOW - border_x - player->rect.w - 1;

    if (player->y <= border_y) player->y = border_y + 1;
    if (player->y >= H_WINDOW - border_y - player->rect.h) player->y = H_WINDOW - border_y - player->rect.h - 1;

    player->rect.x = (int)player->x;
    player->rect.y = (int)player->y;
}