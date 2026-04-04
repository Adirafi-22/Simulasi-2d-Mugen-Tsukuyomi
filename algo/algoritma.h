#ifndef ALGORITMA_H
#define ALGORITMA_H

#include "raylib.h"

// --- ALGORITMA GARIS (BRESENHAM & DDA LINE) ---
void BresenhamLine(int x1, int y1, int x2, int y2, Color color);
void Bres_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color);
void DDALine(int x1, int y1, int x2, int y2, Color color);

// --- ALGORITMA LINGKARAN (MIDPOINT) ---
void Midcircle(int centerX, int centerY, int radius, Color color);
void MidcircleFilled(int centerX, int centerY, int radius, Color color);
void MidcircleThick(int centerX, int centerY, int radius, int thickness, Color color);

// --- ALGORITMA ELIPS ---
void BresenhamEllipse(int cx, int cy, int rx, int ry, Color color);
void MidpointEllipse(int cx, int cy, int rx, int ry, Color color);
void MidpointEllipseFilled(int cx, int cy, int rx, int ry, Color color);

// Kurva Bezier (Untuk Ekor Kurama, Akar Pohon, Tomoe)
void DrawBezierCubic(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void DrawBezierQuadratic(int x0, int y0, int x1, int y1, int x2, int y2, Color color);

// Rotasi Titik 2D (Untuk memutar Tomoe & Pedang Susanoo)
void RotatePoint(float *x, float *y, float cx, float cy, float angle);

#endif