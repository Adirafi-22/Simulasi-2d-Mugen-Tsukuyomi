#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    SCREEN_MENU = 0,
    SCREEN_ANIMASI,
    SCREEN_POLA,
    SCREEN_ABOUT
} GameScreen;

GameScreen UpdateDrawMenuScreen(Font font, Texture2D bg, int screenWidth, int screenHeight, bool showOutline, GameScreen currentScreen, bool *isMenuBgmOn);

#endif