#include <SDL2/SDL.h>

int isMouseInside(SDL_Rect rect, int x, int y);
void drawHealthBar(SDL_Renderer* renderer, int x, int y, int w, int h, int health, int maxHealth);