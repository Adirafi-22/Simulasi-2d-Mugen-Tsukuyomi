#ifndef ANIMASI_H
#define ANIMASI_H

#include "raylib.h"

void InitAnimasiScene(int screenWidth, int screenHeight);
void DrawAnimasiScene(int screenWidth, int screenHeight, float elapsedTime, bool *isAnimBgmOn, Font font);
void UpdateAndDrawBintangAwan(int screenWidth, int screenHeight);
void UpdateAndDrawDebu(int screenWidth, int screenHeight);

#endif