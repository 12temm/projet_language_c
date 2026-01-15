#include "utils.h"

int isMouseInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

void drawHealthBar(SDL_Renderer* renderer, int x, int y, int w, int h, int health, int maxHealth) {
    SDL_Rect bgRect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    if (health > 0) {
        int barWidth = (int)((float)health / maxHealth * w);
        SDL_Rect healthRect = {x, y, barWidth, h};
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &healthRect);
    }
}