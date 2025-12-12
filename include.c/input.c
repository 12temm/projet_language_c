#include "input.h"

void updatePlayer(Chara* player, const Uint8* state, KeyConfig* keys) {
    player->vx = 0;
    player->vy = 0;

    if (state[keys->left]) {
        player->vx -= player->speed;
        player->orientation = 2;
    }
    if (state[keys->right]) {
        player->vx += player->speed;
        player->orientation = 1;
    }
    if (state[keys->up]) {
        player->vy -= player->speed;
    }
    if (state[keys->down]) {
        player->vy += player->speed;
    }

    if (player->vx != 0 && player->vy != 0) {
        player->vx *= 0.7071;
        player->vy *= 0.7071;
    }

    player->x += player->vx;
    player->y += player->vy;

    double bx = W_WINDOW * 0.037;
    double by = H_WINDOW * 0.026;
    double rightLimit = W_WINDOW - bx - player->rect.w;
    double bottomLimit = H_WINDOW - by - player->rect.h;

    if (player->x < bx) player->x = bx + 1;
    if (player->x > rightLimit) player->x = rightLimit - 1;
    if (player->y < by) player->y = by + 1;
    if (player->y > bottomLimit) player->y = bottomLimit - 1;

    player->rect.x = (int)player->x;
    player->rect.y = (int)player->y;
}