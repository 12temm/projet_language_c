#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>


#include "structure.h"
#include "init_global.h"
#include "menu.h"
#include "menu_weapons.h"
#include "background.h"


int isMouseInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void drawHealthBar(SDL_Renderer* renderer, int x, int y, int w, int h, int health, int maxHealth) {
    SDL_Rect bgRect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    if (health > 0) {
        int barWidth = (int)((float)health / maxHealth * w);
        SDL_Rect healthRect = {x, y, barWidth, h};
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &healthRect);
    }
}

int main(int argc, char* argv[]) {

    int inMenu = 1;
    int inMenu2 = 0;
    int inGame = 0;
    int inDeathMenu = 0;

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 24);
    init_global(font);


    Mix_Music* musicGame = Mix_LoadMUS("assets/sounds/game_music.mp3");

    Mix_Music* musicMenu = Mix_LoadMUS("assets/sounds/menu_music.mp3");
    if (!musicMenu) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }
    printf("Playing music...\n");
    Mix_PlayMusic(musicMenu, -1);



    size window_size;

    window_size.height = 800;
    window_size.width = 800;

    int w_window= window_size.width;
    int h_window= window_size.height;

    SDL_Window* window = SDL_CreateWindow("SDL2 Menu",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          w_window, h_window,
                                          SDL_WINDOW_SHOWN);
    window_error(window);


    SDL_Color color = { 255, 255, 255 };

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Noob 32", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    Mix_Music* musicDeath = Mix_LoadMUS("assets/sounds/death_music.mp3");


    SDL_Surface* background = IMG_Load("assets/images/background_game.png");
    surface_background_check(window, renderer, texture, background);
    SDL_Texture *texture_background = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);
    texture_background_check(window, renderer, texture, texture_background);

    SDL_Surface* object_background = IMG_Load("assets/images/object.png");
    surface_background_check(window, renderer, texture, object_background);
    SDL_Texture *texture_object_background = SDL_CreateTextureFromSurface(renderer, object_background);
    SDL_FreeSurface(object_background);
    texture_background_check(window, renderer, texture, texture_object_background);




    SDL_Surface* gameoverSurface = IMG_Load("assets/images/game_over.png");
    if (!gameoverSurface) {
        printf("IMG_Load error: %s\n", IMG_GetError());
    }
    SDL_Texture* texture_gameover = SDL_CreateTextureFromSurface(renderer, gameoverSurface);
    SDL_FreeSurface(gameoverSurface);

    SDL_Rect rect_gameover;
    rect_gameover.x = 0;
    rect_gameover.y = 0;
    rect_gameover.w = w_window;
    rect_gameover.h = h_window;





    #define NUM_ENNEMI 20
    Char ennemis[NUM_ENNEMI];

    for (int i = 0; i < NUM_ENNEMI; i++) {
        ennemis[i].moving = 0;
        ennemis[i].vx = 0;
        ennemis[i].vy = 0;
        ennemis[i].speed = 1;
        ennemis[i].rect.w = 15;
        ennemis[i].rect.h = 15;
        ennemis[i].orientation = 1;

        int side = rand() % 4;

        switch (side) {
            case 0:
                ennemis[i].x = rand() % 800;
            ennemis[i].y = -(rand () % 1000);;
            break;
            case 1:
                ennemis[i].x = rand() % 800;
            ennemis[i].y = 800 + rand () % 1000;
            break;
            case 2:
                ennemis[i].x = -(rand () % 1000);
            ennemis[i].y = rand() % 800;
            break;
            case 3:
                ennemis[i].x = 800 + rand () % 1000;
            ennemis[i].y = rand() % 800;
            break;
        }


        SDL_Surface* mechant_right = IMG_Load("assets/images/mechantR.png");
        ennemis[i].textureR = SDL_CreateTextureFromSurface(renderer, mechant_right);
        SDL_FreeSurface(mechant_right);

        SDL_Surface* mechant_left = IMG_Load("assets/images/mechantL.png");
        ennemis[i].textureL = SDL_CreateTextureFromSurface(renderer, mechant_left);
        SDL_FreeSurface(mechant_left);
    }

    Char player;

    player.health = 3;
    player.moving = 0;
    player.x = 300;
    player.y = 300;
    player.vx = 0;
    player.vy = 0;
    player.speed = 2;
    player.rect.w = 20;
    player.rect.h = 20;
    player.orientation = 1;
    player.currentFrame = 0;
    player.lastFrameTime = 0;
    player.frameDelay = 100;
    player.maxHealth = 3;
    player.invincible = 0;
    player.invincibleStart = 0;
    player.invincibleTime = 1000;



    SDL_Surface* bonhomme_right = IMG_Load("assets/images/bonhommeR.png");
    player.textureR = SDL_CreateTextureFromSurface(renderer, bonhomme_right);
    SDL_FreeSurface(bonhomme_right);

    SDL_Surface* bonhomme_left = IMG_Load("assets/images/bonhommeL.png");
    player.textureL = SDL_CreateTextureFromSurface(renderer, bonhomme_left);
    SDL_FreeSurface(bonhomme_left);

    SDL_Surface* bonhomme_right2 = IMG_Load("assets/images/bonhomme2R.png");
    player.textureR2 = SDL_CreateTextureFromSurface(renderer, bonhomme_right2);
    SDL_FreeSurface(bonhomme_right2);

    SDL_Surface* bonhomme_left2 = IMG_Load("assets/images/bonhomme2L.png");
    player.textureL2 = SDL_CreateTextureFromSurface(renderer, bonhomme_left2);
    SDL_FreeSurface(bonhomme_left2);

    SDL_Surface* bonhomme_right3 = IMG_Load("assets/images/bonhomme3R.png");
    player.textureR3 = SDL_CreateTextureFromSurface(renderer, bonhomme_right3);
    SDL_FreeSurface(bonhomme_right3);

    SDL_Surface* bonhomme_left3 = IMG_Load("assets/images/bonhomme3L.png");
    player.textureL3 = SDL_CreateTextureFromSurface(renderer, bonhomme_left3);
    SDL_FreeSurface(bonhomme_left3);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Button playButton = {{220, 150, 200, 60}, {0, 200, 0, 255}, "Play",1};
    Button quitButton = {{220, 250, 200, 60}, {200, 0, 0, 255}, "Quit",1};

    SDL_Surface* playSurface = TTF_RenderText_Blended(font, playButton.label, color);
    SDL_Texture* playText = SDL_CreateTextureFromSurface(renderer, playSurface);
    SDL_FreeSurface(playSurface);

    SDL_Surface* quitSurface = TTF_RenderText_Blended(font, quitButton.label, color);
    SDL_Texture* quitText = SDL_CreateTextureFromSurface(renderer, quitSurface);
    SDL_FreeSurface(quitSurface);


    SDL_Rect rect;


    int running = 1;
    SDL_Event e;

    while (running) {


        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = 0;

            if (inGame) {
                player.vx = 0;
                player.vy = 0;

                if (state[SDL_SCANCODE_A]) {
                    player.vx -= player.speed;
                    player.orientation = 2;
                }
                if (state[SDL_SCANCODE_D]) {
                    player.vx += player.speed;
                    player.orientation = 1;
                }
                if (state[SDL_SCANCODE_W]) player.vy -= player.speed;
                if (state[SDL_SCANCODE_S]) player.vy += player.speed;

                float len = sqrtf(player.vx * player.vx + player.vy * player.vy);
                if (len > 0) {
                    player.vx = (player.vx / len) * player.speed;
                    player.vy = (player.vy / len) * player.speed;
                }

                if (player.vx != 0 || player.vy != 0) {
                    player.moving = 1;
                } else {
                    player.moving = 0;
                }
            }


            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;
                Button swordButton ={{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword",1};
                Button gunButton ={{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun",1};

                if (inMenu && isMouseInside(playButton.rect, mx, my)) {
                    inMenu = 0;
                    inMenu2 = 1;
                    playButton.visible = 0;
                    quitButton.visible = 0;

                    Mix_HaltMusic();
                    Mix_PlayMusic(musicGame, -1);
                }

                else if (inMenu2 && isMouseInside(swordButton.rect, mx, my)) {
                    inMenu2 = 0;
                    inGame = 1;
                }

                else if (inMenu2 && isMouseInside(gunButton.rect, mx, my)) {
                    inMenu2 = 0;
                    inGame = 1;
                }

                else if (isMouseInside(quitButton.rect, mx, my)) {
                    running = 0;
                }
            }
        }

        if (inGame) {
            for (int i = 0; i < NUM_ENNEMI; i++) {
                float dx = player.x - ennemis[i].x;
                float dy = player.y - ennemis[i].y;

                float dist = sqrtf(dx*dx + dy*dy);
                Uint32 now = SDL_GetTicks();

                if (dist < 10 && !player.invincible) {
                    player.health = player.health -1;
                    player.invincible = 1;
                    player.invincibleStart = now;
                }

                if (dist > 0.1f) {
                    dx /= dist;
                    dy /= dist;

                    ennemis[i].vx = dx * ennemis[i].speed;
                    ennemis[i].vy = dy * ennemis[i].speed;

                    ennemis[i].orientation = (dx >= 0) ? 1 : 2;
                }



                ennemis[i].x += ennemis[i].vx;
                ennemis[i].y += ennemis[i].vy;

                ennemis[i].rect.x = (int)ennemis[i].x;
                ennemis[i].rect.y = (int)ennemis[i].y;
            }

            if (player.invincible) {
                Uint32 now = SDL_GetTicks();
                if (now - player.invincibleStart >= player.invincibleTime) {
                    player.invincible = 0;
                }
            }

        }



        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        if (playButton.visible) {
            if (isMouseInside(playButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(renderer, &playButton.rect);

            SDL_Rect playRectText = {0, 0, 0, 0};
            SDL_QueryTexture(playText, NULL, NULL, &playRectText.w, &playRectText.h);
            playRectText.x = playButton.rect.x + (playButton.rect.w - playRectText.w) / 2;
            playRectText.y = playButton.rect.y + (playButton.rect.h - playRectText.h) / 2;
            SDL_RenderCopy(renderer, playText, NULL, &playRectText);
        }



        if (quitButton.visible) {
            if (isMouseInside(quitButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
            SDL_RenderFillRect(renderer, &quitButton.rect);

            SDL_Rect quitRectText = {0, 0, 0, 0};
            SDL_QueryTexture(quitText, NULL, NULL, &quitRectText.w, &quitRectText.h);
            quitRectText.x = quitButton.rect.x + (quitButton.rect.w - quitRectText.w) / 2;
            quitRectText.y = quitButton.rect.y + (quitButton.rect.h - quitRectText.h) / 2;
            SDL_RenderCopy(renderer, quitText, NULL, &quitRectText);
        }

       if (inMenu2 == 1) {
           choose_weapon(window, font, color, renderer, mx, my);
       }
        if (inGame == 1) {

            if (player.moving) {
                Uint32 now = SDL_GetTicks();
                if (now - player.lastFrameTime > player.frameDelay) {
                    player.currentFrame = (player.currentFrame + 1) % 3;
                    player.lastFrameTime = now;
                }
            } else {
                player.currentFrame = 0;
            }



            show_background(window, renderer, texture,texture_background, rect);


            double border_x = w_window*3.7/100;
            if (player.x <= border_x) {
                player.x = border_x + 1;
            }
            border_x = w_window*6/100;
            if (player.x >= w_window-border_x) {
                player.x = w_window-border_x - 1 ;
            }
            double border_y = h_window*2.6/100;
            if (player.y <= border_y) {
                player.y = border_y + 1 ;
            }
            border_y = h_window*3.6/100;
            if (player.y >= h_window-border_y) {
                player.y = w_window-border_y - 1 ;
            }

            player.x += player.vx;
            player.y += player.vy;

            player.rect.x = (int)player.x;
            player.rect.y = (int)player.y;

            if (player.orientation == 1) {
                if (player.currentFrame == 0) SDL_RenderCopy(renderer, player.textureR, NULL, &player.rect);
                else if (player.currentFrame == 1) SDL_RenderCopy(renderer, player.textureR2, NULL, &player.rect);
                else SDL_RenderCopy(renderer, player.textureR3, NULL, &player.rect);
            } else {
                if (player.currentFrame == 0) SDL_RenderCopy(renderer, player.textureL, NULL, &player.rect);
                else if (player.currentFrame == 1) SDL_RenderCopy(renderer, player.textureL2, NULL, &player.rect);
                else SDL_RenderCopy(renderer, player.textureL3, NULL, &player.rect);
            }


            for (int i = 0; i < NUM_ENNEMI; i++) {
                if (ennemis[i].orientation == 1)
                    SDL_RenderCopy(renderer, ennemis[i].textureR, NULL, &ennemis[i].rect);
                else
                    SDL_RenderCopy(renderer, ennemis[i].textureL, NULL, &ennemis[i].rect);
            }

            show_background(window, renderer, texture,texture_object_background, rect);

            drawHealthBar(renderer, 20, 20, 200, 20, player.health, player.maxHealth);

            if (player.health <= 0) {
                Mix_HaltMusic();
                Mix_PlayMusic(musicDeath, -1);
                inDeathMenu = 1;
                inGame = 0;
            }
        }

        if (inDeathMenu == 1) {
            SDL_RenderCopy(renderer, texture_gameover, NULL, &rect_gameover);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    Mix_HaltMusic();
    Mix_FreeMusic(musicMenu);
    Mix_CloseAudio();
    SDL_DestroyTexture(playText);
    SDL_DestroyTexture(quitText);
    SDL_DestroyTexture(texture);
    destroy_menu_weapons(window,font,color);
    SDL_DestroyTexture(player.textureR);
    SDL_DestroyTexture(player.textureL);
    SDL_DestroyTexture(player.textureR2);
    SDL_DestroyTexture(player.textureL2);
    SDL_DestroyTexture(player.textureR3);
    SDL_DestroyTexture(player.textureL3);
    for (int i = 0; i < NUM_ENNEMI; i++) {
        SDL_DestroyTexture(ennemis[i].textureR);
        SDL_DestroyTexture(ennemis[i].textureL);
    }
    destroy_background(window,renderer,texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}