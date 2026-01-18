#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "settings_menu.h"
#include "structure.h"
#include "init_global.h"
#include "menu.h"
#include "menu_weapons.h"
#include "background.h"
#include "entity.h"
#include "leaderboard.h"
#include "victory.h"
#include "game_logic.h"
#include "config_loader.h"
#include "utils.h"
#include "init_assets.h"
#include "cleanup.h"
#include "draw.h"

int main(int argc, char* argv[]) {



    int inMenu = 1;
    int inMenu2 = 0;
    int inGame = 0;
    int inGame2 = 0;
    int inDeathMenu = 0;
    int inWinMenu = 0;
    int pickedSword = 0;
    int pickedGun = 0;
    int enemies_killed = 0;
    int inSettings = 0;
    int isPaused = 0;
    int fromPause = 0;

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 24);
    init_global(font);

    Mix_Music* musicGame = Mix_LoadMUS("assets/sounds/game_music.mp3");
    Mix_Music* musicMenu = Mix_LoadMUS("assets/sounds/menu_music.mp3");
    Mix_Music* musicVictory = Mix_LoadMUS("assets/sounds/victory_music.mp3");

    if (!musicMenu) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }
    Mix_PlayMusic(musicMenu, -1);

    Config config;
    loadConfig("assets/config.txt", &config);

    int w_window= 800;
    int h_window= 800;
    
    Uint32 lastShotTime = 0;
    int gunCooldown = 1000 / config.attckSpeed;

    SDL_Window* window = SDL_CreateWindow("Super Pixel Survivor",
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


    SDL_Surface* background_world_2 = IMG_Load("assets/images/carte_2.png");
    surface_background_check(window, renderer, texture, background_world_2);
    SDL_Texture *texture_background_world_2 = SDL_CreateTextureFromSurface(renderer, background_world_2);
    SDL_FreeSurface(background_world_2);
    texture_background_check(window, renderer, texture, texture_background_world_2);

    SDL_Surface* projectil_boss_vertical = IMG_Load("assets/images/projectil_boss_vertical.png");
    surface_background_check(window, renderer, texture, projectil_boss_vertical);
    SDL_Texture *texture_projectil_boss_vertical = SDL_CreateTextureFromSurface(renderer, projectil_boss_vertical);
    SDL_FreeSurface(projectil_boss_vertical);
    texture_background_check(window, renderer, texture, texture_projectil_boss_vertical);

    SDL_Surface* projectil_boss_horizontal = IMG_Load("assets/images/projectil_boss_horizontal.png");
    surface_background_check(window, renderer, texture, projectil_boss_horizontal);
    SDL_Texture *texture_projectil_boss_horizontal = SDL_CreateTextureFromSurface(renderer, projectil_boss_horizontal);
    SDL_FreeSurface(projectil_boss_horizontal);
    texture_background_check(window, renderer, texture, texture_projectil_boss_horizontal);

    SDL_Surface* tempettte_boss = IMG_Load("assets/images/tempette_boss.png");
    surface_background_check(window, renderer, texture, tempettte_boss);
    SDL_Texture *texture_tempettte_boss= SDL_CreateTextureFromSurface(renderer, tempettte_boss);
    SDL_FreeSurface(tempettte_boss);
    texture_background_check(window, renderer, texture, texture_tempettte_boss);

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
    init_enemy_assets(renderer, ennemis, config.numEnnemi);

    resetEnnemis(ennemis, config.numEnnemi);
    for (int k = 0; k < config.numEnnemi; k++) {
        ennemis[k].dead = 0;
        if (ennemis[k].type == 1) {
            SDL_Surface* tmpTankR = IMG_Load("assets/images/mechant_tank1R.png");
            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpTankR);
            SDL_FreeSurface(tmpTankR);
            SDL_Surface* tmpTankL = IMG_Load("assets/images/mechant_tank1L.png");
            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpTankL);
            SDL_FreeSurface(tmpTankL);
        } else {
            SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
            SDL_FreeSurface(tmpR);
            SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
            SDL_FreeSurface(tmpL);
        }
    }

    Char player;
    init_player_assets(renderer, &player);

    Char arme;
    init_weapon_assets(renderer, &arme, &player);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Button playButton = {{220, 150, 200, 60}, {0, 200, 0, 255}, "Play",1};
    Button quitButton = {{220, 250, 200, 60}, {200, 0, 0, 255}, "Quit",1};
    Button replayButton = {{570, 600, 200, 60}, {0, 200, 0, 255}, "Rejouer",1};
    Button menuButton = {{570, 700, 200, 60}, {0, 200, 0, 255}, "menu",1};
    Button scoresButton = {{220, 350, 200, 60}, {0, 0, 200, 255}, "Scores", 1};
    Button scoresDeathButton = {{570, 500, 200, 60}, {0, 0, 200, 255}, "Scores", 1};
    Button settingsButton = {{220, 450, 200, 60}, {100, 100, 100, 255}, "Options", 1};

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

    SDL_Surface* settingsSurface = TTF_RenderText_Blended(font, settingsButton.label, color);
    SDL_Texture* settingsText = SDL_CreateTextureFromSurface(renderer, settingsSurface);
    SDL_FreeSurface(settingsSurface);

    SDL_Rect rect;

    int running = 1;
    SDL_Event e;

    while (running) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = 0;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                if (inGame) {
                    isPaused = !isPaused;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && inGame && pickedGun && !isPaused) {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastShotTime >= gunCooldown) {
                    player_shoot_logic(balles, NUM_BALLES, &player, e.button.x, e.button.y, balleTexture);
                    lastShotTime = currentTime;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (inGame && isPaused) {
                    int bw = 200;
                    int bh = 50;
                    int bx = w_window / 2 - bw / 2;
                    int by = h_window / 2 - 100;

                    SDL_Rect btnResume   = {bx, by, bw, bh};
                    SDL_Rect btnSettings = {bx, by + 60, bw, bh};
                    SDL_Rect btnScores   = {bx, by + 120, bw, bh};
                    SDL_Rect btnQuit     = {bx, by + 180, bw, bh};

                    if (isMouseInside(btnResume, mx, my)) {
                        isPaused = 0;
                    }
                    else if (isMouseInside(btnSettings, mx, my)) {
                        fromPause = 1;
                        isPaused = 0;
                        inGame = 0;
                        inMenu = 0;
                        inSettings = 1;
                    }
                    else if (isMouseInside(btnScores, mx, my)) {
                        show_leaderboard_loop(window, renderer, font);
                    }
                    else if (isMouseInside(btnQuit, mx, my)) {
                        isPaused = 0;
                        inGame = 0;
                        inMenu = 1;
                        Mix_HaltMusic();
                        Mix_PlayMusic(musicMenu, -1);
                        resetEnnemis(ennemis, config.numEnnemi);
                        player.x = 300; player.y = 300; player.health = player.maxHealth;
                    }
                    continue;
                }
                if (inMenu && isMouseInside(settingsButton.rect, mx, my)) {
                    fromPause = 0;
                    inMenu = 0;
                    inSettings = 1;
                }

                else if (inSettings) {
                    SDL_Rect btnMinus = {220, 200, 60, 60}; 
                    SDL_Rect btnPlus  = {460, 200, 60, 60};
                    SDL_Rect btnBack  = {220, 400, 300, 60};

                    if (isMouseInside(btnMinus, mx, my)) {
                        int v = Mix_VolumeMusic(-1) - 16;
                        if (v < 0) v = 0;
                        Mix_VolumeMusic(v);
                        Mix_Volume(-1, v);
                    }
                    else if (isMouseInside(btnPlus, mx, my)) {
                        int v = Mix_VolumeMusic(-1) + 16;
                        if (v > 128) v = 128;
                        Mix_VolumeMusic(v);
                        Mix_Volume(-1, v);
                    }
                    else if (isMouseInside(btnBack, mx, my)) {
                        inSettings = 0;

                        if (fromPause == 1) {

                            inGame = 1;
                            isPaused = 1;
                            fromPause = 0;
                        } else {

                            inMenu = 1;
                        }
                    }
                }
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
                    lastShotTime = 0;
                }

                else if (inMenu2 && isMouseInside(gunButton.rect, mx, my)) {
                    inMenu2 = 0;
                    inGame = 1;
                    pickedSword = 0;
                    pickedGun = 1;
                    lastShotTime = 0;
                }

                else if ((isMouseInside(quitButton.rect, mx, my))&& (inMenu)) {
                    running = 0;
                }
                else if (isMouseInside(scoresButton.rect, mx, my) && inMenu) {
                    int ret = show_leaderboard_loop(window, renderer, font);
                    if (ret == 0) running = 0;
                }
                else if (isMouseInside(settingsButton.rect, mx, my) && inMenu) {
                    inMenu = 0;
                    inSettings = 1;
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
                    enemies_killed = 0;
                    lastShotTime = 0;
                    resetEnnemis(ennemis, config.numEnnemi);
                    for (int k = 0; k < config.numEnnemi; k++) {
                        ennemis[k].dead = 0;
                        if (ennemis[k].type == 1) {
                            SDL_Surface* tmpTankR = IMG_Load("assets/images/mechant_tank1R.png");
                            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpTankR);
                            SDL_FreeSurface(tmpTankR);
                            SDL_Surface* tmpTankL = IMG_Load("assets/images/mechant_tank1L.png");
                            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpTankL);
                            SDL_FreeSurface(tmpTankL);
                        } else {
                            SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
                            ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
                            SDL_FreeSurface(tmpR);
                            SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
                            ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
                            SDL_FreeSurface(tmpL);
                        }
                    }
                    for (int k = 0; k < NUM_BALLES; k++) {
                        balles[k].active = 0;
                    }
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
                            if (ennemis[k].type == 1) {
                                SDL_Surface* tmpTankR = IMG_Load("assets/images/mechant_tank1R.png");
                                ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpTankR);
                                SDL_FreeSurface(tmpTankR);
                                SDL_Surface* tmpTankL = IMG_Load("assets/images/mechant_tank1L.png");
                                ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpTankL);
                                SDL_FreeSurface(tmpTankL);
                            } else {
                                SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
                                ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
                                SDL_FreeSurface(tmpR);
                                SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
                                ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
                                SDL_FreeSurface(tmpL);
                            }
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
                        lastShotTime = 0;
                        resetEnnemis(ennemis, config.numEnnemi);
                        for (int k = 0; k < config.numEnnemi; k++) {
                            ennemis[k].dead = 0;
                            if (ennemis[k].type == 1) {
                                SDL_Surface* tmpTankR = IMG_Load("assets/images/mechant_tank1R.png");
                                ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpTankR);
                                SDL_FreeSurface(tmpTankR);
                                SDL_Surface* tmpTankL = IMG_Load("assets/images/mechant_tank1L.png");
                                ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpTankL);
                                SDL_FreeSurface(tmpTankL);
                            } else {
                                SDL_Surface* tmpR = IMG_Load("assets/images/mechantR.png");
                                ennemis[k].textureR = SDL_CreateTextureFromSurface(renderer, tmpR);
                                SDL_FreeSurface(tmpR);
                                SDL_Surface* tmpL = IMG_Load("assets/images/mechantL.png");
                                ennemis[k].textureL = SDL_CreateTextureFromSurface(renderer, tmpL);
                                SDL_FreeSurface(tmpL);
                            }
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

        if (inGame && !isPaused) {
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
            int kills = check_collisions_logic(ennemis, config.numEnnemi, balles, NUM_BALLES, &arme, &player, pickedSword, pickedGun);
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
                Mix_PlayMusic(musicVictory, -1);
            }

            if (player.health <= 0) {
                Mix_HaltMusic();
                Mix_PlayMusic(musicDeath, -1);
                update_high_score("Player", enemies_killed);
                inDeathMenu = 1;
                inGame = 0;
            }
        }

        if (inGame) {
            render_game_scene(renderer, &player, ennemis, &arme, config.numEnnemi,
                              balles, NUM_BALLES,
                              texture_background, texture_object_background,
                              pickedSword, config.attckSpeed, font, enemies_killed);

            if (isPaused) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
                SDL_Rect overlay = {0, 0, w_window, h_window};
                SDL_RenderFillRect(renderer, &overlay);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                int bw = 200;
                int bh = 50;
                int bx = w_window / 2 - bw / 2;
                int by = h_window / 2 - 100;

                SDL_Rect btnResume   = {bx, by, bw, bh};
                SDL_Rect btnSettings = {bx, by + 60, bw, bh};
                SDL_Rect btnScores   = {bx, by + 120, bw, bh};
                SDL_Rect btnQuit     = {bx, by + 180, bw, bh};

                SDL_Color white = {255, 255, 255, 255};

                if (isMouseInside(btnResume, mx, my)) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_RenderFillRect(renderer, &btnResume);

                SDL_Surface* sResume = TTF_RenderText_Blended(font, "Reprendre", white);
                SDL_Texture* tResume = SDL_CreateTextureFromSurface(renderer, sResume);
                SDL_Rect rResume = {0, 0, sResume->w, sResume->h};
                rResume.x = btnResume.x + (btnResume.w - rResume.w) / 2;
                rResume.y = btnResume.y + (btnResume.h - rResume.h) / 2;
                SDL_RenderCopy(renderer, tResume, NULL, &rResume);
                SDL_FreeSurface(sResume); SDL_DestroyTexture(tResume);

                if (isMouseInside(btnSettings, mx, my)) SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                else SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &btnSettings);

                SDL_Surface* sSet = TTF_RenderText_Blended(font, "Parametres", white);
                SDL_Texture* tSet = SDL_CreateTextureFromSurface(renderer, sSet);
                SDL_Rect rSet = {0, 0, sSet->w, sSet->h};
                rSet.x = btnSettings.x + (btnSettings.w - rSet.w) / 2;
                rSet.y = btnSettings.y + (btnSettings.h - rSet.h) / 2;
                SDL_RenderCopy(renderer, tSet, NULL, &rSet);
                SDL_FreeSurface(sSet); SDL_DestroyTexture(tSet);

                if (isMouseInside(btnScores, mx, my)) SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
                else SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
                SDL_RenderFillRect(renderer, &btnScores);

                SDL_Surface* sScore = TTF_RenderText_Blended(font, "Scores", white);
                SDL_Texture* tScore = SDL_CreateTextureFromSurface(renderer, sScore);
                SDL_Rect rScore = {0, 0, sScore->w, sScore->h};
                rScore.x = btnScores.x + (btnScores.w - rScore.w) / 2;
                rScore.y = btnScores.y + (btnScores.h - rScore.h) / 2;
                SDL_RenderCopy(renderer, tScore, NULL, &rScore);
                SDL_FreeSurface(sScore); SDL_DestroyTexture(tScore);

                if (isMouseInside(btnQuit, mx, my)) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                else SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                SDL_RenderFillRect(renderer, &btnQuit);

                SDL_Surface* sQuit = TTF_RenderText_Blended(font, "Quitter", white);
                SDL_Texture* tQuit = SDL_CreateTextureFromSurface(renderer, sQuit);
                SDL_Rect rQuit = {0, 0, sQuit->w, sQuit->h};
                rQuit.x = btnQuit.x + (btnQuit.w - rQuit.w) / 2;
                rQuit.y = btnQuit.y + (btnQuit.h - rQuit.h) / 2;
                SDL_RenderCopy(renderer, tQuit, NULL, &rQuit);
                SDL_FreeSurface(sQuit); SDL_DestroyTexture(tQuit);
            }
        }

        if (inGame2)

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

            if (isMouseInside(settingsButton.rect, mx, my))
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            else
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

            SDL_RenderFillRect(renderer, &settingsButton.rect);

            SDL_Rect settingsRectText = {0, 0, 0, 0};
            SDL_QueryTexture(settingsText, NULL, NULL, &settingsRectText.w, &settingsRectText.h);
            settingsRectText.x = settingsButton.rect.x + (settingsButton.rect.w - settingsRectText.w) / 2;
            settingsRectText.y = settingsButton.rect.y + (settingsButton.rect.h - settingsRectText.h) / 2;
            SDL_RenderCopy(renderer, settingsText, NULL, &settingsRectText);
            
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        }

        if (inMenu2 == 1) {
            choose_weapon(window, font, color, renderer, mx, my);
        }
        if (inSettings == 1) {
            render_settings_menu(renderer, font, mx, my, &inSettings, &inMenu);
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

    cleanup_game(window, renderer, font, musicMenu, ennemis,
                 playText, quitText, texture,
                 texture_background, texture_object_background, texture_gameover,
                 balleTexture, scoresText);

    return 0;
}