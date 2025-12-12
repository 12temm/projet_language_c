#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "button.h"
#include "defs.h" 

typedef struct {
    SDL_Texture *tAudio, *tKeyb, *tBack;
    SDL_Texture *tKeyUp, *tKeyDown, *tKeyLeft, *tKeyRight;
    SDL_Rect rKeyUp, rKeyDown, rKeyLeft, rKeyRight;
    Button btnToAudio, btnToKeys, btnRetGlobal;
    int currentPage; 
    int fromGame;
    int rebindingId; 
} SettingsContext;

void initSettings(SDL_Renderer* renderer, TTF_Font* font, SettingsContext* ctx, KeyConfig* keys);
void handleSettingsEvents(SettingsContext* ctx, int mx, int my, int* inSettings, int* inMenu, int* inGame, int* isPaused, int* volume);
void handleRebind(SettingsContext* ctx, SDL_Scancode key, KeyConfig* keys, SDL_Renderer* ren, TTF_Font* font);
void drawSettings(SDL_Renderer* renderer, SettingsContext* ctx, TTF_Font* font, int volume);
void cleanSettings(SettingsContext* ctx);