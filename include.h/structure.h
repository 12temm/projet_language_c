#include <SDL2/SDL.h>

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
    int moving;
    int health;
    int maxHealth;
    SDL_Texture* textureR;
    SDL_Texture* textureL;
    SDL_Texture* textureR2;
    SDL_Texture* textureL2;
    SDL_Texture* textureR3;
    SDL_Texture* textureL3;
    SDL_Rect rect;
    int currentFrame;
    Uint32 lastFrameTime;
    Uint32 frameDelay;
} Char;