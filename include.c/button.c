#include "button.h"
#include "Utils.h" // Pour utiliser isMouseInside

void drawColoredButton(SDL_Renderer* renderer, Button btn, SDL_Texture* textTex, int mx, int my, SDL_Color hoverCol) {
    if (!btn.visible) return;

    if (isMouseInside(btn.rect, mx, my))
        SDL_SetRenderDrawColor(renderer, hoverCol.r, hoverCol.g, hoverCol.b, 255);
    else
        SDL_SetRenderDrawColor(renderer, btn.color.r, btn.color.g, btn.color.b, 255);

    SDL_RenderFillRect(renderer, &btn.rect);

    if (textTex) {
        SDL_Rect textRect = {0, 0, 0, 0};
        SDL_QueryTexture(textTex, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = btn.rect.x + (btn.rect.w - textRect.w) / 2;
        textRect.y = btn.rect.y + (btn.rect.h - textRect.h) / 2;
        SDL_RenderCopy(renderer, textTex, NULL, &textRect);
    }
}

void drawImageButton(SDL_Renderer* renderer, SDL_Rect rect, SDL_Texture* normal, SDL_Texture* hover, int mx, int my) {
    if (isMouseInside(rect, mx, my)) {
        SDL_RenderCopy(renderer, hover, NULL, &rect);
    } else {
        SDL_RenderCopy(renderer, normal, NULL, &rect);
    }
}