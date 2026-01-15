#include <SDL2/SDL.h>

typedef struct button {
    SDL_Rect rect;
    SDL_Color color;
    char label[50];
    int visible;
} Button;

typedef struct character {
    float x, y;
    float vx, vy;
    int health;
    int maxHealth;
    int speed;
    int moving;
    int orientation;
    int currentFrame;
    Uint32 lastFrameTime;
    int frameDelay;
    int invincible;
    Uint32 invincibleStart;
    int invincibleTime;
    SDL_Rect rect;
    SDL_Texture* textureR;
    SDL_Texture* textureL;
    SDL_Texture* textureR2;
    SDL_Texture* textureL2;
    SDL_Texture* textureR3;
    SDL_Texture* textureL3;
    SDL_Texture* normal_texture;
    int dead;
} Char;

typedef struct config {
    int windowHeight;
    int windowWidth;
    int attckSpeed;
    int numEnnemi;
} Config;

typedef struct projectile {
    float x, y;
    float vx, vy;
    int active;
    float speed;
    SDL_Rect rect;
    SDL_Texture* texture;
} Proj;