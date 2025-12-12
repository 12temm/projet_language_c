#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

Button createSettingsButton(int x, int y);
void drawSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, int mx, int my, Button btnReturn, int currentVolume);
int getVolumeFromClick(int mx, int my);