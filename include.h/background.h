#include <SDL2/SDL.h>


int surface_background_check(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Surface* background);
int texture_background_check(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Texture *texture_background);

int show_background(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture,SDL_Texture *texture_background, SDL_Rect rect);

void destroy_background(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);