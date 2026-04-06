#ifndef POLA_H
#define POLA_H

#include "raylib.h"

Color LerpColor(Color a, Color b, float t);

// Global settings
extern float g_animSpeed;
extern float g_sharinganDir;
extern bool g_isWireframe;
extern float g_accumulatedTime;
extern bool g_showJTK;
extern float g_jtkSmokeTimer;


void DrawBackgroundGradient(int width, int height);
void DrawBulanBiasa(int cx, int cy, int radius, float rotasi, float glowIntensitas, float alphaFading);
void DrawBulanMerah(int cx, int cy, int radius, float rotasi, float glowIntensitas, float alphaFading);
void DrawPohonShinju(int baseX, int baseY, float scale);
void DrawGunungForeground(int screenWidth, int screenHeight);

void DrawBulanAnimasi(int cx, int cy, int radius, float elapsedTime);
void DrawMadaraRikudou(int cx, int cy, float scale, float elapsedTime);
void DrawNarutoKurama(int cx, int cy, float scale, float elapsedTime);
void DrawSasukeSusanoo(int cx, int cy, float scale, float elapsedTime);

void DrawJTKClones(int startX, int startY, float scale, float elapsedTime);

#endif