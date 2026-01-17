#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include "structure.h"


void resetEnnemis(Char ennemis[], int count) {
    for (int i = 0; i < count; i++) {
        ennemis[i].moving = 0;
        ennemis[i].vx = 0;
        ennemis[i].vy = 0;
        switch (ennemis[i].type) {
            case 1:
                ennemis[i].health = 3;
                ennemis[i].maxHealth = 3;
                ennemis[i].speed = 1;
                ennemis[i].rect.w = 25;
                ennemis[i].rect.h = 25;
                break;
            case 0:
            default:
                ennemis[i].health = 1;
                ennemis[i].maxHealth = 1;
                ennemis[i].speed = 1;
                ennemis[i].rect.w = 15;
                ennemis[i].rect.h = 15;
                break;
        }

        int side = rand() % 4;
        switch (side) {
            case 0:
                ennemis[i].x = rand() % 800;
            ennemis[i].y = -(rand() % 1000);
            break;
            case 1:
                ennemis[i].x = rand() % 800;
            ennemis[i].y = 800 + rand() % 1000;
            break;
            case 2:
                ennemis[i].x = -(rand() % 1000);
            ennemis[i].y = rand() % 800;
            break;
            case 3:
                ennemis[i].x = 800 + rand() % 1000;
            ennemis[i].y = rand() % 800;
            break;
        }

        ennemis[i].rect.x = ennemis[i].x;
        ennemis[i].rect.y = ennemis[i].y;
    }
}
