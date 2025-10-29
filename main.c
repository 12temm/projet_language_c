#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char* label;
} Button;

int isMouseInside(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music* music = Mix_LoadMUS("assets/sounds/game_music.mp3");
    if (!music) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    printf("Playing music...\n");
    Mix_PlayMusic(music, -1);  // Loop forever

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Menu",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --- Define buttons ---
    Button playButton = {{220, 150, 200, 60}, {0, 200, 0, 255}, "Play"};
    Button quitButton = {{220, 250, 200, 60}, {200, 0, 0, 255}, "Quit"};

    int running = 1;
    SDL_Event e;

    while (running) {
        // --- Event handling ---
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;

                if (isMouseInside(playButton.rect, mx, my)) {
                    printf("Play button clicked!\n");
                    // TODO: Start your game here
                } else if (isMouseInside(quitButton.rect, mx, my)) {
                    printf("Quit button clicked!\n");
                    running = 0;
                }
            }
        }

        // --- Rendering ---
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // background
        SDL_RenderClear(renderer);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // Play button
        if (isMouseInside(playButton.rect, mx, my))
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &playButton.rect);

        // Quit button
        if (isMouseInside(quitButton.rect, mx, my))
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &quitButton.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
