#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


#include "defs.h"
#include "init.h"
#include "input.h"
#include "draw.h"
#include "Utils.h"
#include "settings.h"
#include "button.h"
int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initAll(&window, &renderer)) {
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 24);
    if (!font) { printf("Font Error: %s\n", TTF_GetError()); return 1; }

    Mix_Music* musicMenu = Mix_LoadMUS("assets/sounds/menu_music.mp3");
    Mix_Music* musicGame = Mix_LoadMUS("assets/sounds/game_music.mp3");
    if (musicMenu) Mix_PlayMusic(musicMenu, -1);

    SDL_Color white = {255, 255, 255};

    SDL_Surface* tmpSurf = TTF_RenderText_Blended(font, "Play", white);
    SDL_Texture* texPlay = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_FreeSurface(tmpSurf);

    tmpSurf = TTF_RenderText_Blended(font, "Quit", white);
    SDL_Texture* texQuit = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_FreeSurface(tmpSurf);

    tmpSurf = TTF_RenderText_Blended(font, "Parametres", white);
    SDL_Texture* texSettings = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_FreeSurface(tmpSurf);

    SDL_Surface* sBg = IMG_Load("assets/images/background_game.png");
    SDL_Texture* bg = SDL_CreateTextureFromSurface(renderer, sBg);
    SDL_FreeSurface(sBg);

    SDL_Surface* sObj = IMG_Load("assets/images/object.png");
    SDL_Texture* objBg = SDL_CreateTextureFromSurface(renderer, sObj);
    SDL_FreeSurface(sObj);

    SDL_Surface* sKnife = IMG_Load("assets/images/couteau.png");
    SDL_Texture* tKnife = SDL_CreateTextureFromSurface(renderer, sKnife);
    SDL_FreeSurface(sKnife);

    SDL_Surface* sKnifeH = IMG_Load("assets/images/couteauhover.png");
    SDL_Texture* tKnifeH = SDL_CreateTextureFromSurface(renderer, sKnifeH);
    SDL_FreeSurface(sKnifeH);

    SDL_Surface* sGun = IMG_Load("assets/images/pistolet.png");
    SDL_Texture* tGun = SDL_CreateTextureFromSurface(renderer, sGun);
    SDL_FreeSurface(sGun);

    SDL_Surface* sGunH = IMG_Load("assets/images/pistolethover.png");
    SDL_Texture* tGunH = SDL_CreateTextureFromSurface(renderer, sGunH);
    SDL_FreeSurface(sGunH);


    Chara player = {300, 300, 0, 0, 2.0f, 1, NULL, NULL, {300, 300, 20, 20}};
    SDL_Surface* sP1 = IMG_Load("assets/images/bonhommeR.png");
    player.textureR = SDL_CreateTextureFromSurface(renderer, sP1);
    SDL_FreeSurface(sP1);

    SDL_Surface* sP2 = IMG_Load("assets/images/bonhommeL.png");
    player.textureL = SDL_CreateTextureFromSurface(renderer, sP2);
    SDL_FreeSurface(sP2);

    Button btnPlay = {{220, 150, 200, 60}, {0, 200, 0, 255}, "Play", 1};
    Button btnQuit = {{220, 250, 200, 60}, {200, 0, 0, 255}, "Quit", 1};
    Button btnSword = {{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword", 1};
    Button btnGun = {{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun", 1};

    Button btnSettings = createSettingsButton(220, 350);

    int inMenu = 1, inMenu2 = 0, inGame = 0, running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            // Gestion clavier en jeu
            if (inGame && e.type == SDL_KEYUP) {
               if (e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_s) player.vy = 0;
               if (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_d) player.vx = 0;
            }

            // Gestion souris
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;

                if (inMenu) {
                    // Clic sur PLAY
                    if (isMouseInside(btnPlay.rect, mx, my)) {
                        inMenu = 0; inMenu2 = 1;
                        btnPlay.visible = 0;
                        btnQuit.visible = 0;
                        btnSettings.visible = 0;
                        Mix_HaltMusic(); Mix_PlayMusic(musicGame, -1);
                    }
                    // Clic sur PARAMETRES
                    else if (isMouseInside(btnSettings.rect, mx, my)) {
                        printf("Clic sur parametres !\n");
                    }
                    // Clic sur QUIT
                    else if (isMouseInside(btnQuit.rect, mx, my)) {
                        running = 0;
                    }
                }
                else if (inMenu2) {
                    if (isMouseInside(btnSword.rect, mx, my)) { inMenu2 = 0; inGame = 1; }
                    else if (isMouseInside(btnGun.rect, mx, my)) { inMenu2 = 0; inGame = 1; }
                }
            }
        }

        if (inGame) {
            updatePlayer(&player, SDL_GetKeyboardState(NULL));
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        if (inMenu) {
            drawColoredButton(renderer, btnPlay, texPlay, mx, my, (SDL_Color){0, 255, 0, 255});
            drawColoredButton(renderer, btnQuit, texQuit, mx, my, (SDL_Color){255, 0, 0, 255});
            drawColoredButton(renderer, btnSettings, texSettings, mx, my, (SDL_Color){80, 80, 255, 255});
        }

        if (inMenu2) {
            drawImageButton(renderer, btnSword.rect, tKnife, tKnifeH, mx, my);
            drawImageButton(renderer, btnGun.rect, tGun, tGunH, mx, my);
        }

        if (inGame) {
            SDL_RenderCopy(renderer, bg, NULL, NULL);
            drawPlayer(renderer, &player);

            SDL_Rect objRect = {0,0,0,0};
            SDL_QueryTexture(objBg, NULL, NULL, &objRect.w, &objRect.h);
            objRect.x = (800 - objRect.w)/2; objRect.y = (800 - objRect.h)/2;
            SDL_RenderCopy(renderer, objBg, NULL, &objRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup(window, renderer, font, musicMenu, musicGame);
    return 0;
}