#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void init_leaderboard();

void update_high_score(const char* player_name, int score);

int show_leaderboard_loop(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);