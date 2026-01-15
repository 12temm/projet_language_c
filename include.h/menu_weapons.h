#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int choose_weapon(SDL_Window* window, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer, int mx, int my);
void destroy_menu_weapons(SDL_Window* window, TTF_Font* font, SDL_Color color);