#include "structure.h"
#include "game_logic.h"
#include <math.h>
#include <stdio.h>


void update_player_logic(Char *player, const Uint8 *state, int w_window, int h_window) {
    player->vx = 0;
    player->vy = 0;

    if (state[SDL_SCANCODE_A]) {
        player->vx -= player->speed;
        player->orientation = 2;
    }
    if (state[SDL_SCANCODE_D]) {
        player->vx += player->speed;
        player->orientation = 1;
    }
    if (state[SDL_SCANCODE_W]) player->vy -= player->speed;
    if (state[SDL_SCANCODE_S]) player->vy += player->speed;

    float len = sqrtf(player->vx * player->vx + player->vy * player->vy);
    if (len > 0) {
        player->vx = (player->vx / len) * player->speed;
        player->vy = (player->vy / len) * player->speed;
    }

    player->moving = (player->vx != 0 || player->vy != 0) ? 1 : 0;


    double border_x = w_window*3.7/100;
    if (player->x <= border_x) player->x = border_x + 1;

    double border_x_right = w_window*6/100;
    if (player->x >= w_window-border_x_right) player->x = w_window-border_x_right - 1;

    double border_y = h_window*2.6/100;
    if (player->y <= border_y) player->y = border_y + 1;

    double border_y_bottom = h_window*3.6/100;
    if (player->y >= h_window-border_y_bottom) player->y = w_window-border_y_bottom - 1;

    player->x += player->vx;
    player->y += player->vy;
    player->rect.x = (int)player->x;
    player->rect.y = (int)player->y;
}


void update_enemies_logic(Char *ennemis, int numEnnemi, Char *player) {
    Uint32 now = SDL_GetTicks();
    for (int i = 0; i < numEnnemi; i++) {
        if (!ennemis[i].dead) {
            float dx = player->x - ennemis[i].x;

            if (dx >= 0)
                ennemis[i].orientation = 1; //
            else
                ennemis[i].orientation = 0;

            float dy = player->y - ennemis[i].y;
            float dist = sqrtf(dx*dx + dy*dy);


            if (dist < 10 && !player->invincible) {
                player->health -= 1;
                player->invincible = 1;
                player->invincibleStart = now;
            }

            if (dist > 0.1f) {
                dx /= dist; dy /= dist;
                ennemis[i].vx = dx * ennemis[i].speed;
                ennemis[i].vy = dy * ennemis[i].speed;
            }
            ennemis[i].x += ennemis[i].vx;
            ennemis[i].y += ennemis[i].vy;
            ennemis[i].rect.x = (int)ennemis[i].x;
            ennemis[i].rect.y = (int)ennemis[i].y;
        }else {
            ennemis[i].vx = 0;
            ennemis[i].vy = 0;
        }

    }
    if (player->invincible && (now - player->invincibleStart >= player->invincibleTime)) {
        player->invincible = 0;
    }
}


void update_bullets_logic(Proj *balles, int numBalles, int w_window, int h_window) {
    for (int i = 0; i < numBalles; i++) {
        if (balles[i].active) {
            balles[i].x += balles[i].vx;
            balles[i].y += balles[i].vy;
            balles[i].rect.x = (int)balles[i].x;
            balles[i].rect.y = (int)balles[i].y;

            if (balles[i].x < 0 || balles[i].x > w_window || balles[i].y < 0 || balles[i].y > h_window) {
                balles[i].active = 0;
            }
        }
    }
}

void player_shoot_logic(Proj *balles, int numBalles, Char *player, int mx, int my, SDL_Texture *balleTexture) {
    for (int i = 0; i < numBalles; i++) {
        if (!balles[i].active) {
            balles[i].active = 1;

            balles[i].x = player->x + player->rect.w / 2;
            balles[i].y = player->y + player->rect.h / 2;

            float dx = mx - balles[i].x;
            float dy = my - balles[i].y;

            float length = sqrtf(dx * dx + dy * dy);
            if (length == 0) length = 1;

            balles[i].speed = 8;
            balles[i].vx = (dx / length) * balles[i].speed;
            balles[i].vy = (dy / length) * balles[i].speed;

            balles[i].rect.w = 10;
            balles[i].rect.h = 4;
            balles[i].rect.x = (int)balles[i].x;
            balles[i].rect.y = (int)balles[i].y;

            balles[i].texture = balleTexture;
            break;
        }
    }
}

int check_collisions_logic(Char *ennemis, int numEnnemi, Proj *balles, int numBalles, Char *arme, Char *player, int pickedSword, int pickedGun) {
    int kills = 0;

    if (pickedSword) {
        for (int i = 0; i < numEnnemi; i++) {
            if (!ennemis[i].dead) {
                SDL_Rect rArme = {arme->rect.x, arme->rect.y, arme->rect.w, arme->rect.h};
                SDL_Rect rEnnemi = {ennemis[i].rect.x, ennemis[i].rect.y, ennemis[i].rect.w, ennemis[i].rect.h};

                if (SDL_HasIntersection(&rEnnemi, &rArme) && arme->type == 1) {

                    ennemis[i].health -= 1;
                    ennemis[i].x += (ennemis[i].x - player->x) * 0.5f;
                    ennemis[i].y += (ennemis[i].y - player->y) * 0.5f;

                    if (ennemis[i].health <= 0) {
                        ennemis[i].x = 1000;
                        ennemis[i].y = 1000;
                        ennemis[i].dead = 1;
                        ennemis[i].vx = 0;
                        ennemis[i].vy = 0;
                        kills++;
                    }
                }
            }
        }
    }


    if (pickedGun) {
        for (int i = 0; i < numEnnemi; i++) {
            if (!ennemis[i].dead) {
                for (int j = 0; j < numBalles; j++) {
                    if (balles[j].active) {
                        SDL_Rect rBalle = {balles[j].rect.x, balles[j].rect.y, balles[j].rect.w, balles[j].rect.h};
                        SDL_Rect rEnnemi = {ennemis[i].rect.x, ennemis[i].rect.y, ennemis[i].rect.w, ennemis[i].rect.h};

                        if (SDL_HasIntersection(&rEnnemi, &rBalle)) {
                            balles[j].active = 0;
                            ennemis[i].health -= 1;

                            if (ennemis[i].health <= 0) {
                                ennemis[i].x = 1000;
                                ennemis[i].y = 1000;
                                ennemis[i].dead = 1;
                                kills++;
                            }
                        }
                    }
                }
            }
        }
    }
    return kills;
}