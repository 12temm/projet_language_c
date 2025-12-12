#include "settings.h"

Button createSettingsButton(int x, int y) {
    Button btn;
    btn.rect.x = x;
    btn.rect.y = y;
    btn.rect.w = 200;
    btn.rect.h = 60;

    // Couleur bleue
    btn.color = (SDL_Color){0, 0, 200, 255};

    btn.label = "Parametres";
    btn.visible = 1;

    return btn;
}