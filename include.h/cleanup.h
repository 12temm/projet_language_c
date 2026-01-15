#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

struct character;

void cleanup_game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font,
                  Mix_Music* music, struct character* ennemis,
                  SDL_Texture* t1, SDL_Texture* t2, SDL_Texture* t3,
                  SDL_Texture* t4, SDL_Texture* t5, SDL_Texture* t6,
                  SDL_Texture* t7, SDL_Texture* t8);