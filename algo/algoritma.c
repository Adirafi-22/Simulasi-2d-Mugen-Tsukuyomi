#include "algoritma.h"
#include <math.h>
#include <stdlib.h>

// =======================================================================
// ALGORITMA GARIS (BRESENHAM & DDA LINE)
// =======================================================================

void BresenhamLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2*err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void Bres_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;
    float px = -dy/len, py = dx/len;
    int half = thick/2;
    for (int t = -half; t <= half; t++) {
        int ox = (int)roundf(px*t), oy = (int)roundf(py*t);
        BresenhamLine(x1+ox, y1+oy, x2+ox, y2+oy, color);
    }
}

void DDALine(int x1, int y1, int x2, int y2, Color color) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    if (steps == 0) { DrawPixel(x1, y1, color); return; }
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        DrawPixel((int)roundf(x), (int)roundf(y), color);
        x += xInc; y += yInc;
    }
}

// =======================================================================
// ALGORITMA LINGKARAN (MIDPOINT)
// =======================================================================

static void PlotSymmetryPoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color); // Oktan 1
    DrawPixel(cx + y, cy + x, color); // Oktan 2
    DrawPixel(cx + y, cy - x, color); // Oktan 3
    DrawPixel(cx + x, cy - y, color); // Oktan 4
    DrawPixel(cx - x, cy - y, color); // Oktan 5
    DrawPixel(cx - y, cy - x, color); // Oktan 6
    DrawPixel(cx - y, cy + x, color); // Oktan 7
    DrawPixel(cx - x, cy + y, color); // Oktan 8
}

void Midcircle(int centerX, int centerY, int radius, Color color) {
    if (radius <= 0) { DrawPixel(centerX, centerY, color); return; }
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;  
    
    while (y >= x) {
        PlotSymmetryPoints(centerX, centerY, x, y, color);
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void MidcircleFilled(int centerX, int centerY, int radius, Color color) {
    if (radius <= 0) { DrawPixel(centerX, centerY, color); return; }
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (y >= x) {
        for (int i = centerX - x; i <= centerX + x; i++) {
            DrawPixel(i, centerY + y, color);
            DrawPixel(i, centerY - y, color);
        }
        for (int i = centerX - y; i <= centerX + y; i++) {
            DrawPixel(i, centerY + x, color);
            DrawPixel(i, centerY - x, color);
        }
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void MidcircleThick(int centerX, int centerY, int radius, int thickness, Color color) {
    if (radius <= 0 || thickness <= 0) { DrawPixel(centerX, centerY, color); return; }
    int innerR = radius - thickness / 2;
    if (innerR < 0) innerR = 0;
    int outerR = radius + thickness / 2;
    for (int r = innerR; r <= outerR; r++) {
        Midcircle(centerX, centerY, r, color);
    }
}

// =======================================================================
// ALGORITMA ELIPS
// =======================================================================

static void DrawEllipsePoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color);  // Kuadran I
    DrawPixel(cx - x, cy + y, color);  // Kuadran II
    DrawPixel(cx + x, cy - y, color);  // Kuadran IV
    DrawPixel(cx - x, cy - y, color);  // Kuadran III
}

void BresenhamEllipse(int cx, int cy, int rx, int ry, Color color) {
    if (rx == 0 || ry == 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;

    int x = 0;
    int y = ry;

    long d1 = ry2 - rx2 * ry + rx2 / 4;

    while (ry2 * x < rx2 * y) {
        DrawEllipsePoints(cx, cy, x, y, color);

        x++;
        if (d1 < 0) {
            d1 += ry2 * (2 * x + 3);
        } else {
            y--;
            d1 += ry2 * (2 * x + 3) - rx2 * (2 * y + 2);
        }
    }

    long d2 = ry2 * (x * x + x) + rx2 * (y * y - 2 * y + 1) - rx2 * ry2;

    while (y >= 0) {
        DrawEllipsePoints(cx, cy, x, y, color);

        y--;
        if (d2 > 0) {
            d2 += rx2 * (3 - 2 * y);
        } else {
            x++;
            d2 += ry2 * (2 * x + 2) + rx2 * (3 - 2 * y);
        }
    }
}

void MidpointEllipse(int cx, int cy, int rx, int ry, Color color) {
    if (rx == 0 || ry == 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;

    int x = 0;
    int y = ry;

    long p1 = 4 * ry2 - 4 * rx2 * ry + rx2;

    while (ry2 * x < rx2 * y) {
        DrawEllipsePoints(cx, cy, x, y, color);

        x++;
        if (p1 < 0) {
            p1 += 4 * ry2 * (2 * x + 3);
        } else {
            y--;
            p1 += 4 * ry2 * (2 * x + 3) - 4 * rx2 * (2 * y + 2);
        }
    }

    long p2 = ry2 * (2 * x + 1) * (2 * x + 1) + 4 * rx2 * (y - 1) * (y - 1) - 4 * rx2 * ry2;

    while (y >= 0) {
        DrawEllipsePoints(cx, cy, x, y, color);

        y--;
        if (p2 > 0) {
            p2 += 4 * rx2 * (-2 * y + 3);
        } else {
            x++;
            p2 += 4 * ry2 * (2 * x + 2) + 4 * rx2 * (-2 * y + 3);
        }
    }
}

static void DrawEllipsePointsFilled(int cx, int cy, int x, int y, Color color) {
    BresenhamLine(cx - x, cy + y, cx + x, cy + y, color);
    BresenhamLine(cx - x, cy - y, cx + x, cy - y, color);
}

void MidpointEllipseFilled(int cx, int cy, int rx, int ry, Color color) {
    if (rx == 0 || ry == 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;

    int x = 0;
    int y = ry;

    long p1 = 4 * ry2 - 4 * rx2 * ry + rx2;

    while (ry2 * x < rx2 * y) {
        DrawEllipsePointsFilled(cx, cy, x, y, color);

        x++;
        if (p1 < 0) {
            p1 += 4 * ry2 * (2 * x + 3);
        } else {
            y--;
            p1 += 4 * ry2 * (2 * x + 3) - 4 * rx2 * (2 * y + 2);
        }
    }

    long p2 = ry2 * (2 * x + 1) * (2 * x + 1) + 4 * rx2 * (y - 1) * (y - 1) - 4 * rx2 * ry2;

    while (y >= 0) {
        DrawEllipsePointsFilled(cx, cy, x, y, color);

        y--;
        if (p2 > 0) {
            p2 += 4 * rx2 * (-2 * y + 3);
        } else {
            x++;
            p2 += 4 * ry2 * (2 * x + 2) + 4 * rx2 * (-2 * y + 3);
        }
    }
}

// =======================================================================
// ALGORITMA KURVA & TRANSFORMASI
// =======================================================================

// Kurva Bezier Quadratic (3 Titik)
void DrawBezierQuadratic(int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    float step = 0.01f; // Semakin kecil, semakin halus kurvanya
    int prevX = x0;
    int prevY = y0;
    
    for (float t = 0; t <= 1.0f; t += step) {
        float u = 1.0f - t;
        int nextX = (int)(u*u*x0 + 2*u*t*x1 + t*t*x2);
        int nextY = (int)(u*u*y0 + 2*u*t*y1 + t*t*y2);
        BresenhamLine(prevX, prevY, nextX, nextY, color);
        prevX = nextX;
        prevY = nextY;
    }
    BresenhamLine(prevX, prevY, x2, y2, color); // Tutup kurva
}

// Kurva Bezier Cubic (4 Titik)
void DrawBezierCubic(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
    float step = 0.01f; 
    int prevX = x0;
    int prevY = y0;
    
    for (float t = 0; t <= 1.0f; t += step) {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        int nextX = (int)(uuu * x0 + 3 * uu * t * x1 + 3 * u * tt * x2 + ttt * x3);
        int nextY = (int)(uuu * y0 + 3 * uu * t * y1 + 3 * u * tt * y2 + ttt * y3);
        
        BresenhamLine(prevX, prevY, nextX, nextY, color);
        prevX = nextX;
        prevY = nextY;
    }
    BresenhamLine(prevX, prevY, x3, y3, color);
}

// Rotasi Titik 2D 
void RotatePoint(float *x, float *y, float cx, float cy, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    // Translasi titik ke titik 
    float px = *x - cx;
    float py = *y - cy;

    // Rotasi
    float xnew = px * c - py * s;
    float ynew = px * s + py * c;

    // Translasi kembali
    *x = xnew + cx;
    *y = ynew + cy;
}