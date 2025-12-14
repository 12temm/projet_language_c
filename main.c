#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char* label;
    int visible;
} Button;

typedef struct {
    float x, y;
    float vx, vy;
    float speed;
    int orientation;
    SDL_Texture* textureR;
    SDL_Texture* textureL;
    SDL_Rect rect;
} Char;

int isMouseInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

int main(int argc, char* argv[]) {

    int inMenu = 1;
    int inMenu2 = 0;
    int inGame = 0;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music* musicMenu = Mix_LoadMUS("assets/sounds/menu_music.mp3");
    Mix_Music* musicGame = Mix_LoadMUS("assets/sounds/game_music.mp3");

    if (!musicMenu) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    printf("Playing music...\n");
    Mix_PlayMusic(musicMenu, -1);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    int w_window = 800;
    int h_window = 800;
    SDL_Window* window = SDL_CreateWindow("SDL2 Menu",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          w_window, h_window,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Init();

    TTF_Font* font = TTF_OpenFont("assets/fonts/Open_Sans/OpenSans-VariableFont_wdth,wght.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color color = { 255, 255, 255 };

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Noob 32", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Surface* surfaceCouteau = IMG_Load("assets/images/couteau.png");
    SDL_Texture* textureCouteau = SDL_CreateTextureFromSurface(renderer, surfaceCouteau);
    SDL_FreeSurface(surfaceCouteau);
    SDL_Surface* surfaceCouteauHover = IMG_Load("assets/images/couteauhover.png");
    SDL_Texture* textureCouteauHover = SDL_CreateTextureFromSurface(renderer, surfaceCouteauHover);
    SDL_FreeSurface(surfaceCouteauHover);

    SDL_Surface* surfacePistolet = IMG_Load("assets/images/pistolet.png");
    SDL_Texture* texturePistolet = SDL_CreateTextureFromSurface(renderer, surfacePistolet);
    SDL_FreeSurface(surfacePistolet);
    SDL_Surface* surfacePistoletHover = IMG_Load("assets/images/pistolethover.png");
    SDL_Texture* texturePistoletHover = SDL_CreateTextureFromSurface(renderer, surfacePistoletHover);
    SDL_FreeSurface(surfacePistoletHover);

    SDL_Surface* background = IMG_Load("assets/images/background_game.png");
    if (background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_background = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);
    if(texture_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Surface* object_background = IMG_Load("assets/images/object.png");
    if (object_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *texture_object_background = SDL_CreateTextureFromSurface(renderer, object_background);
    SDL_FreeSurface(object_background);
    if(texture_object_background == NULL) {
        SDL_Log("ERREUR IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }


    Char player;

    player.x = 300;
    player.y = 300;
    player.vx = 0;
    player.vy = 0;
    player.speed = 2.0f;
    player.rect.w = 20;
    player.rect.h = 20;

    SDL_Surface* bonhomme_right = IMG_Load("assets/images/bonhommeR.png");
    player.textureR = SDL_CreateTextureFromSurface(renderer, bonhomme_right);
    SDL_FreeSurface(bonhomme_right);

    SDL_Surface* bonhomme_left = IMG_Load("assets/images/bonhommeL.png");
    player.textureL = SDL_CreateTextureFromSurface(renderer, bonhomme_left);
    SDL_FreeSurface(bonhomme_left);

    player.vx = 0;
    player.vy = 0;

    player.orientation = 1;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Button playButton = {{220, 150, 200, 60}, {0, 200, 0, 255}, "Play",1};
    Button quitButton = {{220, 250, 200, 60}, {200, 0, 0, 255}, "Quit",1};
    Button swordButton ={{100, 100, 200, 200}, {0, 200, 0, 255}, "Sword",1};
    Button gunButton ={{300, 100, 200, 200}, {0, 200, 0, 255}, "Gun",1};



    SDL_Rect rect;

    SDL_Surface* playSurface = TTF_RenderText_Blended(font, playButton.label, color);
    SDL_Texture* playText = SDL_CreateTextureFromSurface(renderer, playSurface);
    SDL_FreeSurface(playSurface);

    SDL_Surface* quitSurface = TTF_RenderText_Blended(font, quitButton.label, color);
    SDL_Texture* quitText = SDL_CreateTextureFromSurface(renderer, quitSurface);
    SDL_FreeSurface(quitSurface);

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
            }


            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;

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



        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // background
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
        if (isMouseInside(swordButton.rect, mx, my)) {
            SDL_RenderCopy(renderer, textureCouteauHover, NULL, &swordButton.rect);
        } else {
            SDL_RenderCopy(renderer, textureCouteau, NULL, &swordButton.rect);
        }

        if (isMouseInside(gunButton.rect, mx, my)) {
            SDL_RenderCopy(renderer, texturePistoletHover, NULL, &gunButton.rect);
        } else {
            SDL_RenderCopy(renderer, texturePistolet, NULL, &gunButton.rect);
        }

       }
        if (inGame == 1) {
            if (SDL_QueryTexture(texture_background,NULL,NULL, &rect.w, &rect.h) != 0) {
                SDL_Log("ERREUR > %s\n", SDL_GetError());
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                exit(EXIT_FAILURE);
            }

            rect.x = (800 -rect.w)/2;
            rect.y = (800 -rect.h)/2;

            SDL_RenderCopy(renderer, texture_background, NULL, &rect);


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
                SDL_RenderCopy(renderer, player.textureR, NULL, &player.rect);
            }
            else if (player.orientation == 2) {
                SDL_RenderCopy(renderer, player.textureL, NULL, &player.rect);
            }


            if (SDL_QueryTexture(texture_object_background,NULL,NULL, &rect.w, &rect.h) != 0) {
                SDL_Log("ERREUR > %s\n", SDL_GetError());
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                SDL_Quit();
                exit(EXIT_FAILURE);
            }

            rect.x = (800 -rect.w)/2;
            rect.y = (800 -rect.h)/2;

            SDL_RenderCopy(renderer, texture_object_background, NULL, &rect);

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
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}