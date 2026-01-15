#include <SDL2/SDL.h>

struct character;
struct projectile;

void update_player_logic(struct character *player, const Uint8 *state, int w_window, int h_window);
void update_enemies_logic(struct character *ennemis, int numEnnemi, struct character *player);
void update_bullets_logic(struct projectile *balles, int numBalles, int w_window, int h_window);
void player_shoot_logic(struct projectile *balles, int numBalles, struct character *player, int mx, int my, SDL_Texture *balleTexture);
int check_collisions_logic(struct character *ennemis, int numEnnemi, struct projectile *balles, int numBalles, struct character *arme, struct character *player, int pickedSword, int pickedGun);