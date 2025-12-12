#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

void drawSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, int mx, int my, Button btnRet, int volume);
int getVolumeFromClick(int mx, int my);
Button createSettingsButton(int x, int y);