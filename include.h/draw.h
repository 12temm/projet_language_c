#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct character;
struct projectile;

void render_game_scene(SDL_Renderer* renderer, struct character* player, struct character* ennemis, struct character* arme, int numEnnemi,
                       struct projectile* balles, int numBalles,
                       SDL_Texture* texBg, SDL_Texture* texObj,
                       int pickedSword, int attackSpeed,
                       TTF_Font* font, int enemies_killed);