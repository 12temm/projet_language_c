#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "structure.h"
#include "init_global.h"
#include "menu.h"
#include "menu_weapons.h"
#include "background.h"
#include "entity.h"
#include "leaderboard.h"
#include "victory.h"
#include "game_logic.h"

int isMouseInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
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

void loadConfig(char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Le fichier de config (%s) n'a pas pu etre lu .\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* chariot = strchr(line, '\n');
        if(chariot != NULL) {
            *chariot = '\0';
        }
        char *equalSign = strchr(line, '=');
        if (equalSign == NULL) {
            continue;
        }
        *equalSign = '\0';
        char *key = line;
        char *value = equalSign + 1;
        if (strcmp(key, "WINDOW_HEIGHT") == 0) {
            config->windowHeight = atoi(value);
        }
        if (strcmp(key, "WINDOW_WIDTH") == 0) {
            config->windowWidth = atoi(value);
        }
        if (strcmp(key, "ATTACK_SPEED") == 0) {
            config->attckSpeed = atoi(value);
        }
        if (strcmp(key, "NUM_ENNEMI") == 0) {
            config->numEnnemi = atoi(value);
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]) {

    int inMenu = 1;
    int inMenu2 = 0;
    int inGame = 0;
    int inDeathMenu = 0;
    int inWinMenu = 0;
    int pickedSword = 0;
    int pickedGun = 0;
    int enemies_killed = 0;

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
    Mix_PlayMusic(musicMenu, -1);

    Config config;
    loadConfig("assets/config.txt", &config);

    int w_window= config.windowWidth;
    int h_window= config.windowHeight;

    SDL_Window* window = SDL_CreateWindow("quoicoubeh",
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

    #define NUM_BALLES 20
    Proj balles[NUM_BALLES];

    for (int i = 0; i < NUM_BALLES; i++) {
        balles[i].active = 0;
    }

    SDL_Surface* balleSurface = IMG_Load("assets/images/balle.png");
    if (!balleSurface) {
        printf("Erreur IMG_Load balle: %s\n", IMG_GetError());
    }

    SDL_Texture* balleTexture = SDL_CreateTextureFromSurface(renderer, balleSurface);
    SDL_FreeSurface(balleSurface);

    Char* ennemis = NULL;
    ennemis = malloc(sizeof(Char) * config.numEnnemi);
    if (ennemis == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return 1;
    }
    resetEnnemis(ennemis, config.numEnnemi);
    for (int i = 0; i < config.numEnnemi; i++) {
        ennemis[i].dead = 0;
    }
    for (int i = 0; i <config.numEnnemi; i++) {
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

    Char arme;
    arme.x = player.x + 50;
    arme.y = player.y + 50;
    arme.rect.w = player.rect.w;
    arme.rect.h = player.rect.h+50;

    SDL_Surface* couteau = IMG_Load("assets/images/couteau.png");
    SDL_Surface* armeS = IMG_Load("assets/images/arme.png");

    arme.normal_texture = SDL_CreateTextureFromSurface(renderer, armeS);
    SDL_FreeSurface(couteau);
    SDL_FreeSurface(armeS);

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
    Button replayButton = {{570, 600, 200, 60}, {0, 200, 0, 255}, "Reesayer",1};
    Button menuButton = {{570, 700, 200, 60}, {0, 200, 0, 255}, "menu",1};
    Button scoresButton = {{220, 350, 200, 60}, {0, 0, 200, 255}, "Scores", 1};
    Button scoresDeathButton = {{570, 500, 200, 60}, {0, 0, 200, 255}, "Scores", 1};

    SDL_Surface* scoresDeathSurface = TTF_RenderText_Blended(font, scoresDeathButton.label, color);
    SDL_Texture* scoresDeathText = SDL_CreateTextureFromSurface(renderer, scoresDeathSurface);
    SDL_FreeSurface(scoresDeathSurface);

    SDL_Surface* scoresSurface = TTF_RenderText_Blended(font, scoresButton.label, color);
    SDL_Texture* scoresText = SDL_CreateTextureFromSurface(renderer, scoresSurface);
    SDL_FreeSurface(scoresSurface);
    init_leaderboard();

    SDL_Surface* menuSurface = TTF_RenderText_Blended(font, menuButton.label, color);
    SDL_Texture* menuText = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_FreeSurface(menuSurface);

    SDL_Surface* replaySurface = TTF_RenderText_Blended(font, replayButton.label, color);
    SDL_Texture* replayText = SDL_CreateTextureFromSurface(renderer, replaySurface);
    SDL_FreeSurface(replaySurface);

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

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = 0;

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && inGame && pickedGun) {
                player_shoot_logic(balles, NUM_BALLES, &player, e.button.x, e.button.y, balleTexture);
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {

                Button swordButton ={{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword",1};
                Button gunButton ={{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun",1};

                if (inMenu && isMouseInside(playButton.rect, mx, my)) {
                    inMenu = 0;
                    inMenu2 = 1;
                    playButton.visible = 0;
                    quitButton.visible = 0;
                    enemies_killed = 0;

                    Mix_HaltMusic();
                    Mix_PlayMusic(musicGame, -1);
                }

                else if (inMenu2 && isMouseInside(swordButton.rect, mx, my)) {
                    inMenu2 = 0;
                    inGame = 1;
                    pickedSword = 1;
                    pickedGun = 0;
                }

                else if (inMenu2 && isMouseInside(gunButton.rect, mx, my)) {
                    inMenu2 = 0;
                    inGame = 1;
                    pickedSword = 0;
                    pickedGun = 1;
                }

                else if ((isMouseInside(quitButton.rect, mx, my))&& (inMenu)) {
                    running = 0;
                }
                else if (isMouseInside(scoresButton.rect, mx, my) && inMenu) {
                    int ret = show_leaderboard_loop(window, renderer, font);
                    if (ret == 0) running = 0;
                }

                else if ((isMouseInside(menuButton.rect, mx, my))&& (inDeathMenu)){

                    inDeathMenu = 0;
                    inMenu = 1;
                    playButton.visible = 1;
                    quitButton.visible = 1;

                    player.health = player.maxHealth;
                    player.x = 300;
                    player.y = 300;
                    player.vx = 0;
                    player.vy = 0;
                    player.invincible = 0;

                    resetEnnemis(ennemis, config.numEnnemi);

                    Mix_HaltMusic();
                    Mix_PlayMusic(musicGame, -1);
                }

                else if ((isMouseInside(replayButton.rect, mx, my))&& (inDeathMenu)){

                        inDeathMenu = 0;
                        inGame = 1;

                        player.health = player.maxHealth;
                        player.x = 300;
                        player.y = 300;
                        player.vx = 0;
                        player.vy = 0;
                        player.invincible = 0;

                        resetEnnemis(ennemis, config.numEnnemi);

                        Mix_HaltMusic();
                        Mix_PlayMusic(musicGame, -1);
                    }
                else if (isMouseInside(scoresDeathButton.rect, mx, my) && inDeathMenu) {
                    show_leaderboard_loop(window, renderer, font);
                }
                else if (inWinMenu) {
                    if ((isMouseInside(menuButton.rect, mx, my)) && (inWinMenu)) {
                        inWinMenu = 0;
                        inMenu = 1;
                        playButton.visible = 1;
                        quitButton.visible = 1;

                        player.health = player.maxHealth;
                        enemies_killed = 0;

                        resetEnnemis(ennemis, config.numEnnemi);

                        for (int k = 0; k < config.numEnnemi; k++) {
                            ennemis[k].dead = 0;
                            SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
                            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
                            SDL_FreeSurface(tmpR);
                            SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
                            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
                            SDL_FreeSurface(tmpL);
                        }

                        for (int k = 0; k < NUM_BALLES; k++) balles[k].active = 0;

                        Mix_HaltMusic();
                        Mix_PlayMusic(musicMenu, -1);
                    }

                   else if ((isMouseInside(replayButton.rect, mx, my)) && (inWinMenu)) {

                        inWinMenu = 0;
                        inGame = 1;

                        player.health = player.maxHealth;
                        player.x = 300;
                        player.y = 300;
                        player.vx = 0;
                        player.vy = 0;
                        player.invincible = 0;

                        enemies_killed = 0;

                        resetEnnemis(ennemis, config.numEnnemi);

                        for (int k = 0; k < config.numEnnemi; k++) {
                            ennemis[k].dead = 0;

                            SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
                            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
                            SDL_FreeSurface(tmpR);

                            SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
                            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
                            SDL_FreeSurface(tmpL);
                        }

                        for (int k = 0; k < NUM_BALLES; k++) {
                            balles[k].active = 0;
                        }

                        Mix_HaltMusic();
                        Mix_PlayMusic(musicGame, -1);
                   }
                   else if ((isMouseInside(scoresDeathButton.rect, mx, my)) && (inWinMenu)) {
                       show_leaderboard_loop(window, renderer, font);
                   }
                }
            }
        }

        if (inGame) {
            if (player.moving) {
                Uint32 now = SDL_GetTicks();
                if (now - player.lastFrameTime > player.frameDelay) {
                    player.currentFrame = (player.currentFrame + 1) % 3;
                    player.lastFrameTime = now;
                }
            } else {
                player.currentFrame = 0;
            }

            update_player_logic(&player, state, w_window, h_window);
            update_enemies_logic(ennemis, config.numEnnemi, &player);
            update_bullets_logic(balles, NUM_BALLES, w_window, h_window);
            int kills = check_collisions_logic(ennemis, config.numEnnemi, balles, NUM_BALLES, &arme, pickedSword, pickedGun);
            enemies_killed += kills;

            int total_dead = 0;
            for(int i=0; i<config.numEnnemi; i++) {
                if(ennemis[i].dead) total_dead++;
            }

            if (total_dead == config.numEnnemi) {
                inGame = 0;
                inWinMenu = 1;
                update_high_score("Player", enemies_killed);
                Mix_HaltMusic();
            }

            show_background(window, renderer, texture,texture_background, rect);

            SDL_Point center;
            center.x = (player.rect.w/2);
            center.y = (player.rect.h/2);
            Uint32 now = SDL_GetTicks()/config.attckSpeed;
            arme.rect.x = (int)player.x;
            arme.rect.y = (int)player.y;

            if (player.orientation == 1) {
                if (player.currentFrame == 0) {
                    SDL_RenderCopy(renderer, player.textureR, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
                else if (player.currentFrame == 1) {
                    SDL_RenderCopy(renderer, player.textureR2, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
                else {
                    SDL_RenderCopy(renderer, player.textureR3, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
            } else {
                if (player.currentFrame == 0) {
                    SDL_RenderCopy(renderer, player.textureL, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
                else if (player.currentFrame == 1) {
                    SDL_RenderCopy(renderer, player.textureL2, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
                else {
                    SDL_RenderCopy(renderer, player.textureL3, NULL, &player.rect);
                    if (pickedSword == 1) {
                        SDL_RenderCopyEx(renderer,arme.normal_texture, NULL, &arme.rect, now, &center, SDL_FLIP_VERTICAL);
                    }
                }
            }

            for (int i = 0; i < config.numEnnemi; i++) {
                if (ennemis[i].orientation == 1 && !ennemis[i].dead)
                    SDL_RenderCopy(renderer, ennemis[i].textureR, NULL, &ennemis[i].rect);
                else if (!ennemis[i].dead)
                    SDL_RenderCopy(renderer, ennemis[i].textureL, NULL, &ennemis[i].rect);
            }

            for (int i = 0; i < NUM_BALLES; i++) {
                if (balles[i].active) {
                    SDL_RenderCopy(renderer, balles[i].texture, NULL, &balles[i].rect);
                }
            }

            show_background(window, renderer, texture,texture_object_background, rect);

            drawHealthBar(renderer, 20, 20, 200, 20, player.health, player.maxHealth);

            char scoreText[32];
            sprintf(scoreText, "Score: %d", enemies_killed);

            SDL_Color scoreColor = {255, 255, 255, 255};
            SDL_Surface* surfaceScore = TTF_RenderText_Blended(font, scoreText, scoreColor);

            if (surfaceScore) {
                SDL_Texture* textureScore = SDL_CreateTextureFromSurface(renderer, surfaceScore);
                SDL_Rect rectScore;
                rectScore.x = 20;
                rectScore.y = 50;
                rectScore.w = surfaceScore->w;
                rectScore.h = surfaceScore->h;

                SDL_RenderCopy(renderer, textureScore, NULL, &rectScore);

                SDL_FreeSurface(surfaceScore);
                SDL_DestroyTexture(textureScore);
            }

            if (player.health <= 0) {
                Mix_HaltMusic();
                Mix_PlayMusic(musicDeath, -1);

                update_high_score("Player", enemies_killed);

                inDeathMenu = 1;
                inGame = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        if(!inGame) SDL_RenderClear(renderer);

        if (inMenu) {
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

            if (isMouseInside(scoresButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
            SDL_RenderFillRect(renderer, &scoresButton.rect);

            SDL_Rect scoresRectText = {0, 0, 0, 0};
            SDL_QueryTexture(scoresText, NULL, NULL, &scoresRectText.w, &scoresRectText.h);
            scoresRectText.x = scoresButton.rect.x + (scoresButton.rect.w - scoresRectText.w) / 2;
            scoresRectText.y = scoresButton.rect.y + (scoresButton.rect.h - scoresRectText.h) / 2;
            SDL_RenderCopy(renderer, scoresText, NULL, &scoresRectText);
        }

        if (inMenu2 == 1) {
            choose_weapon(window, font, color, renderer, mx, my);
        }

        if (inDeathMenu == 1) {
            SDL_RenderCopy(renderer, texture_gameover, NULL, &rect_gameover);
            replayButton.visible = 1;

            if (isMouseInside(replayButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
            else
                SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
            SDL_RenderFillRect(renderer, &replayButton.rect);

            SDL_Rect replayRectText = {0, 0, 0, 0};
            SDL_QueryTexture(replayText, NULL, NULL, &replayRectText.w, &replayRectText.h);
            replayRectText.x = replayButton.rect.x + (replayButton.rect.w - replayRectText.w) / 2;
            replayRectText.y = replayButton.rect.y + (replayButton.rect.h - replayRectText.h) / 2;
            SDL_RenderCopy(renderer, replayText, NULL, &replayRectText);

            menuButton.visible = 1;
            scoresDeathButton.visible = 1;
            if (isMouseInside(scoresDeathButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
            SDL_RenderFillRect(renderer, &scoresDeathButton.rect);

            SDL_Rect scoresDeathRectText = {0, 0, 0, 0};
            SDL_QueryTexture(scoresDeathText, NULL, NULL, &scoresDeathRectText.w, &scoresDeathRectText.h);
            scoresDeathRectText.x = scoresDeathButton.rect.x + (scoresDeathButton.rect.w - scoresDeathRectText.w) / 2;
            scoresDeathRectText.y = scoresDeathButton.rect.y + (scoresDeathButton.rect.h - scoresDeathRectText.h) / 2;
            SDL_RenderCopy(renderer, scoresDeathText, NULL, &scoresDeathRectText);

            if (isMouseInside(menuButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
            else
                SDL_SetRenderDrawColor(renderer, 50, 0, 50, 255);
            SDL_RenderFillRect(renderer, &menuButton.rect);

            SDL_Rect menuRectText = {0, 0, 0, 0};
            SDL_QueryTexture(menuText, NULL, NULL, &menuRectText.w, &menuRectText.h);
            menuRectText.x = menuButton.rect.x + (menuButton.rect.w - menuRectText.w) / 2;
            menuRectText.y = menuButton.rect.y + (menuButton.rect.h - menuRectText.h) / 2;
            SDL_RenderCopy(renderer, menuText, NULL, &menuRectText);

        }
        else if (inWinMenu == 1) {
            replayButton.visible = 1;
            menuButton.visible = 1;
            scoresDeathButton.visible = 1;
            render_victory_screen(renderer, font, w_window, h_window, mx, my, &replayButton, &menuButton, &scoresDeathButton);
        }
        else {
            menuButton.visible = 0;
            replayButton.visible = 0;
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

    SDL_DestroyTexture(arme.normal_texture);
    SDL_DestroyTexture(player.textureR);
    SDL_DestroyTexture(player.textureL);
    SDL_DestroyTexture(player.textureR2);
    SDL_DestroyTexture(player.textureL2);
    SDL_DestroyTexture(player.textureR3);
    SDL_DestroyTexture(player.textureL3);
    for (int i = 0; i < config.numEnnemi; i++) {
        if(ennemis[i].textureR) SDL_DestroyTexture(ennemis[i].textureR);
        if(ennemis[i].textureL) SDL_DestroyTexture(ennemis[i].textureL);
    }
    destroy_background(window,renderer,texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(scoresText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(scoresDeathText);
    SDL_DestroyWindow(window);

    destroy_menu_weapons(window, font, color);
    SDL_Quit();
    free(ennemis);

    return 0;
}