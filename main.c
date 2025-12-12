#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"
#include "button.h"
#include "init.h"
#include "input.h"
#include "draw.h"
#include "Utils.h"
#include "settings.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!initAll(&window, &renderer)) return 1;

    TTF_Font* font = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 24);
    Mix_Music* musicMenu = Mix_LoadMUS("assets/sounds/menu_music.mp3");
    Mix_Music* musicGame = Mix_LoadMUS("assets/sounds/game_music.mp3");

    int volume = 64;
    Mix_VolumeMusic(volume);
    if (musicMenu) Mix_PlayMusic(musicMenu, -1);

    SDL_Color white = {255, 255, 255};
    SDL_Surface* tmp;

    tmp = TTF_RenderText_Blended(font, "Play", white);
    SDL_Texture* tPlay = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(font, "Quit", white);
    SDL_Texture* tQuit = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);
    tmp = TTF_RenderText_Blended(font, "Parametres", white);
    SDL_Texture* tSet = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    SDL_Surface* s;
    s = IMG_Load("assets/images/background_game.png"); SDL_Texture* bg = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/object.png"); SDL_Texture* obj = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/couteau.png"); SDL_Texture* tKnife = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/couteauhover.png"); SDL_Texture* tKnifeH = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/pistolet.png"); SDL_Texture* tGun = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/pistolethover.png"); SDL_Texture* tGunH = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);

    Chara player = {300, 300, 0, 0, 2.0f, 1, NULL, NULL, {300, 300, 20, 20}};
    s = IMG_Load("assets/images/bonhommeR.png"); player.textureR = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);
    s = IMG_Load("assets/images/bonhommeL.png"); player.textureL = SDL_CreateTextureFromSurface(renderer, s); SDL_FreeSurface(s);

    Button btnPlay = {{300, 250, 200, 60}, {0, 200, 0, 255}, "Play", 1};
    Button btnQuit = {{300, 490, 200, 60}, {200, 0, 0, 255}, "Quit", 1};
    Button btnSet = createSettingsButton(300, 370);
    Button btnRet = {{300, 600, 200, 60}, {150, 150, 150, 255}, "Retour", 1};
    Button btnSword = {{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword", 1};
    Button btnGun = {{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun", 1};

    int inMenu = 1, inSettings = 0, inMenu2 = 0, inGame = 0, running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (inGame && e.type == SDL_KEYUP) {
               if (e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_s) player.vy = 0;
               if (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_d) player.vx = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                if (inMenu) {
                    if (isMouseInside(btnPlay.rect, mx, my)) { inMenu = 0; inMenu2 = 1; Mix_HaltMusic(); Mix_PlayMusic(musicGame, -1); }
                    else if (isMouseInside(btnSet.rect, mx, my)) { inMenu = 0; inSettings = 1; }
                    else if (isMouseInside(btnQuit.rect, mx, my)) { running = 0; }
                } else if (inSettings) {
                    if (isMouseInside(btnRet.rect, mx, my)) { inSettings = 0; inMenu = 1; }
                    int nv = getVolumeFromClick(mx, my);
                    if (nv != -1) { volume = nv; Mix_VolumeMusic(volume); }
                } else if (inMenu2) {
                    if (isMouseInside(btnSword.rect, mx, my)) { inMenu2 = 0; inGame = 1; }
                    else if (isMouseInside(btnGun.rect, mx, my)) { inMenu2 = 0; inGame = 1; }
                }
            }
        }

        if (inGame) updatePlayer(&player, SDL_GetKeyboardState(NULL));

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        if (inMenu) {
            drawColoredButton(renderer, btnPlay, tPlay, mx, my, (SDL_Color){0, 255, 0, 255});
            drawColoredButton(renderer, btnQuit, tQuit, mx, my, (SDL_Color){255, 0, 0, 255});
            drawColoredButton(renderer, btnSet, tSet, mx, my, (SDL_Color){80, 80, 255, 255});
        } else if (inSettings) {
            drawSettingsMenu(renderer, font, mx, my, btnRet, volume);
        } else if (inMenu2) {
            drawImageButton(renderer, btnSword.rect, tKnife, tKnifeH, mx, my);
            drawImageButton(renderer, btnGun.rect, tGun, tGunH, mx, my);
        } else if (inGame) {
            SDL_RenderCopy(renderer, bg, NULL, NULL);
            drawPlayer(renderer, &player);
            SDL_Rect rObj = {0,0,0,0}; SDL_QueryTexture(obj, NULL, NULL, &rObj.w, &rObj.h);
            rObj.x = (800 - rObj.w)/2; rObj.y = (800 - rObj.h)/2;
            SDL_RenderCopy(renderer, obj, NULL, &rObj);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    cleanup(window, renderer, font, musicMenu, musicGame);
    return 0;
}