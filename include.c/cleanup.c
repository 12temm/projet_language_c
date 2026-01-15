#include "structure.h"
#include "cleanup.h"
#include "menu_weapons.h"
#include "background.h"
#include <stdlib.h>

void cleanup_game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font,
                  Mix_Music* music, struct character* ennemis,
                  SDL_Texture* playText, SDL_Texture* quitText, SDL_Texture* titleTex,
                  SDL_Texture* bgTex, SDL_Texture* objTex, SDL_Texture* goTex,
                  SDL_Texture* ballTex, SDL_Texture* scoreTex) {

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();

    if(playText) SDL_DestroyTexture(playText);
    if(quitText) SDL_DestroyTexture(quitText);
    if(titleTex) SDL_DestroyTexture(titleTex);
    if(bgTex) SDL_DestroyTexture(bgTex);
    if(objTex) SDL_DestroyTexture(objTex);
    if(goTex) SDL_DestroyTexture(goTex);
    if(ballTex) SDL_DestroyTexture(ballTex);
    if(scoreTex) SDL_DestroyTexture(scoreTex);

    if (ennemis != NULL) {
        free(ennemis);
    }

    destroy_menu_weapons(window, font, (SDL_Color){255,255,255});

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}