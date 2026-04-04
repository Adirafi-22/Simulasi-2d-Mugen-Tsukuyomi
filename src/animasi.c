#include "animasi.h"
#include "pola.h"
#include "algoritma.h"
#include <stdlib.h>
#include <math.h>

#define MAX_STARS 150
#define MAX_CLOUDS 10
#define MAX_DUST 80

typedef struct {
    float x, y;
    float size;
    float speedX, speedY;
    float opacity;
} Particle;

static Particle stars[MAX_STARS];
static Particle clouds[MAX_CLOUDS];
static Particle dust[MAX_DUST];
static bool isInitialized = false;

static float RandFloat(float min, float max) {
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void InitAnimasiScene(int screenWidth, int screenHeight) {
    if (isInitialized) return;

    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x = RandFloat(0.0f, 1.0f); 
        stars[i].y = RandFloat(0.0f, 0.7f); 
        stars[i].size = RandFloat(1.0f, 2.5f);
        stars[i].opacity = RandFloat(0.1f, 1.0f);
        stars[i].speedX = RandFloat(0.01f, 0.05f) * (rand() % 2 == 0 ? 1 : -1);
    }

    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].x = RandFloat(0, screenWidth);
        clouds[i].y = RandFloat(50, screenHeight * 0.5f);
        clouds[i].size = RandFloat(0.8f, 1.5f); 
        clouds[i].speedX = RandFloat(0.3f, 0.8f);
    }

    for (int i = 0; i < MAX_DUST; i++) {
        dust[i].x = RandFloat(0, screenWidth);
        dust[i].y = RandFloat(screenHeight - 200, screenHeight);
        dust[i].size = RandFloat(1.0f, 3.0f);
        dust[i].speedX = RandFloat(-1.0f, 1.0f);
        dust[i].speedY = RandFloat(-1.5f, -0.5f); 
        dust[i].opacity = RandFloat(50, 150); 
    }

    isInitialized = true;
}

void UpdateAndDrawBintangAwan(int screenWidth, int screenHeight) {
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].opacity += stars[i].speedX; 
        if (stars[i].opacity > 1.0f || stars[i].opacity < 0.1f) {
            stars[i].speedX *= -1.0f; 
        }
        unsigned char alpha = (unsigned char)(stars[i].opacity * 255);
        Color starCol = (Color){255, 255, 255, alpha};
        int sx = (int)(stars[i].x * screenWidth);
        int sy = (int)(stars[i].y * screenHeight);
        DrawCircle(sx, sy, (int)stars[i].size, starCol);
    }

    Color cloudCol = (Color){80, 70, 100, 100}; 
    for (int i = 0; i < MAX_CLOUDS; i++) {
        clouds[i].x += clouds[i].speedX; 
        
        if (clouds[i].x > screenWidth + 150) {
            clouds[i].x = -150;
            clouds[i].y = RandFloat(50, screenHeight * 0.5f);
        }

        int cx = (int)clouds[i].x;
        int cy = (int)clouds[i].y;
        float s = clouds[i].size;
        DrawCircle(cx, cy, (int)(30*s), cloudCol);
        DrawCircle(cx + (int)(30*s), cy - (int)(15*s), (int)(35*s), cloudCol);
        MidpointEllipseFilled(cx + (int)(60*s), cy, (int)(27*s), (int)(24*s), cloudCol);
    }
}

void DrawAnimasiScene(int screenWidth, int screenHeight, float elapsedTime, bool *isAnimBgmOn, Font font) {
    DrawBackgroundGradient(screenWidth, screenHeight);

    UpdateAndDrawBintangAwan(screenWidth, screenHeight);

    int bulanX = (int)(screenWidth * (300.0f / 1280.0f));
    DrawBulanAnimasi(bulanX, 200, 120, elapsedTime);

    float hoverY = sinf(elapsedTime * 2.0f) * 15.0f; 
    int madaraX = (int)(screenWidth * (480.0f / 1280.0f));
    DrawMadaraRikudou(madaraX, 240 + (int)hoverY, 0.4f, elapsedTime);

    DrawGunungForeground(screenWidth, screenHeight);

    float maxTreeScale = (float)screenHeight / 720.0f; 
    float t = elapsedTime / 10.0f;
    if (t > 1.0f) t = 1.0f;
    // Ease-Out Sine interpolation untuk pergerakan tumbuh yang smooth
    float treeScale = sinf(t * 3.14159265f / 2.0f) * maxTreeScale; 
    
    int shinjuX = (int)(screenWidth * 0.78f); 
    DrawPohonShinju(shinjuX, screenHeight, treeScale);

    int kuramaX = (int)(screenWidth * (250.0f / 1280.0f));
    int susanooX = (int)(screenWidth * (390.0f / 1280.0f));
    
    DrawSasukeSusanoo(susanooX, screenHeight - 60, 0.9f, elapsedTime);
    DrawNarutoKurama(kuramaX, screenHeight - 60, 0.9f, elapsedTime);

    UpdateAndDrawDebu(screenWidth, screenHeight);

    float btnWidth = 120.0f;
    float btnHeight = 40.0f;
    Rectangle bgmBtn = { screenWidth - btnWidth - 20, 20, btnWidth, btnHeight };
    
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, bgmBtn);
    
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *isAnimBgmOn = !(*isAnimBgmOn);
    }
    
    Color btnBg = (Color){20, 30, 50, 200};
    Color btnHover = MAROON;
    
    DrawRectangleRec(bgmBtn, isHovered ? btnHover : btnBg);
    DrawRectangleLinesEx(bgmBtn, 2, WHITE);
    
    const char *bgmText = *isAnimBgmOn ? "BGM OFF" : "BGM ON";
    float btnFontSize = 20.0f;
    Vector2 textSize = MeasureTextEx(font, bgmText, btnFontSize, 1);
    Vector2 textPos = { bgmBtn.x + (bgmBtn.width - textSize.x) / 2.0f, bgmBtn.y + (bgmBtn.height - textSize.y) / 2.0f };
    DrawTextEx(font, bgmText, textPos, btnFontSize, 1, WHITE);
}

void UpdateAndDrawDebu(int screenWidth, int screenHeight) {
    Color dustCol = (Color){200, 180, 150, 100};
    for (int i = 0; i < MAX_DUST; i++) {
        dust[i].x += dust[i].speedX;
        dust[i].y += dust[i].speedY;

        if (dust[i].y < screenHeight - 250) {
            dust[i].y = screenHeight;
            dust[i].x = RandFloat(0, screenWidth);
        }
        if (dust[i].x > screenWidth) dust[i].x = 0;
        if (dust[i].x < 0) dust[i].x = screenWidth;

        dustCol.a = (unsigned char)dust[i].opacity;
        DrawCircle((int)dust[i].x, (int)dust[i].y, (int)dust[i].size, dustCol);
    }
}