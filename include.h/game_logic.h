void update_player_logic(Char *player, const Uint8 *state, int w_window, int h_window);
void update_enemies_logic(Char *ennemis, int numEnnemi, Char *player);
void update_bullets_logic(Proj *balles, int numBalles, int w_window, int h_window);
void player_shoot_logic(Proj *balles, int numBalles, Char *player, int mx, int my, SDL_Texture *balleTexture);
int check_collisions_logic(Char *ennemis, int numEnnemi, Proj *balles, int numBalles, Char *arme, int pickedSword, int pickedGun);