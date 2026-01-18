#include <SDL2/SDL.h>

void init_player_assets(SDL_Renderer* renderer, Char* player, int config_point_de_vie,int config_point_de_vie_max);
void init_enemy_assets(SDL_Renderer* renderer, Char* ennemis, int numEnnemi);
void init_weapon_assets(SDL_Renderer* renderer, Char* arme, Char* player);