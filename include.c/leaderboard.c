#include "leaderboard.h"
#include <stdio.h>
#include <string.h>

#define MAX_SCORES 5
#define SCORE_FILE "scores.txt"


typedef struct {
    char name[20];
    int score;
} HighScore;

static HighScore scores[MAX_SCORES];


static void save_scores() {
    FILE* f = fopen(SCORE_FILE, "w");
    if (f == NULL) {
        printf("Couldn't open %s\n", SCORE_FILE);
    }
    if (f) {
        for (int i = 0; i < MAX_SCORES; i++) {
            fprintf(f, "%s %d\n", scores[i].name, scores[i].score);
        }
        fclose(f);
    }
}

static void render_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
    if (surf) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {x - surf->w / 2, y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}


void init_leaderboard() {
    FILE* f = fopen(SCORE_FILE, "r");
    if (f == NULL) {
        printf("Couldn't open %s\n", SCORE_FILE);
    }
    if (f) {
        for (int i = 0; i < MAX_SCORES; i++) {
            if (fscanf(f, "%19s %d", scores[i].name, &scores[i].score) != 2) {
                strcpy(scores[i].name, "---");
                scores[i].score = 0;
            }
        }
        fclose(f);
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(scores[i].name, "Vide");
            scores[i].score = 0;
        }
        save_scores();
    }
}

void update_high_score(const char* player_name, int score) {
    if (score <= scores[MAX_SCORES - 1].score) return;

    int i;
    for (i = MAX_SCORES - 1; i > 0; i--) {
        if (score > scores[i - 1].score) {
            scores[i] = scores[i - 1];
        } else {
            break;
        }
    }

    strcpy(scores[i].name, player_name);
    scores[i].score = score;
    save_scores();
}

int show_leaderboard_loop(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    int running = 1;
    SDL_Event event;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color gray = {200, 200, 200, 255};

    SDL_Rect backButtonRect = {50, 700, 150, 50};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0; // On quitte tout le jeu
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                if (mx >= backButtonRect.x && mx <= backButtonRect.x + backButtonRect.w &&
                    my >= backButtonRect.y && my <= backButtonRect.y + backButtonRect.h) {
                    return 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);


        render_text_centered(renderer, font, "MEILLEURS SCORES", 400, 50, gold);

        char buffer[100];
        for (int i = 0; i < MAX_SCORES; i++) {
            sprintf(buffer, "%d. %s : %d", i + 1, scores[i].name, scores[i].score);
            render_text_centered(renderer, font, buffer, 400, 150 + (i * 80), white);
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &backButtonRect);
        render_text_centered(renderer, font, "RETOUR", backButtonRect.x + 75, backButtonRect.y + 10, white);

        SDL_RenderPresent(renderer);
    }
    return 1;
}