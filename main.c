#include "raylib.h"
#include <stdbool.h>
#include <math.h> 

#include "menu.h"
#include "about.h"
#include "pola.h" 
#include "animasi.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    int screenWidth = 1280;
    int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Simulasi Mugen Tsukuyomi");
    SetExitKey(KEY_NULL);

    Texture2D bgMenu = LoadTexture("assets/background_menu.png");
    Texture2D bgAbout = LoadTexture("assets/kaguya.jpg"); 
    Texture2D bgPola = LoadTexture("assets/madara.png");
    Font customFont = LoadFont("assets/njnaruto.ttf"); 
    if (customFont.baseSize == 0) customFont = GetFontDefault();

    InitAudioDevice();
    Music bgMusic = LoadMusicStream("assets/Waltz of Wind and Fire.mp3");
    bgMusic.looping = true;
    SetMusicVolume(bgMusic, 1.2f);
    
    Music menuMusic = LoadMusicStream("assets/Tsukino Ookisa (Off Vocal Version).mp3");
    menuMusic.looping = true;
    SetMusicVolume(menuMusic, 0.3f);
    PlayMusicStream(menuMusic);

    GameScreen currentScreen = SCREEN_MENU;
    bool showOutline = false;
    int polaViewMode = 1; 

    bool isMenuBgmOn = true;
    bool prevMenuBgmOn = true;
    bool isAnimBgmOn = true;
    bool prevAnimBgmOn = true;

    float animStartTime = 0.0f;

    InitAnimasiScene(screenWidth, screenHeight);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackgroundGradient(screenWidth, screenHeight);
    DrawPohonShinju(-2000, -2000, 1.0f);
    DrawBulanBiasa(-2000, -2000, 120, 0, 0.0f, 1.0f);
    DrawBulanMerah(-2000, -2000, 120, 0, 0.0f, 0.0f);
    EndDrawing();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        if (currentScreen != SCREEN_ANIMASI) {
            UpdateMusicStream(menuMusic);
        }

        if (IsKeyPressed(KEY_O)) showOutline = !showOutline;

        if (currentScreen != SCREEN_MENU) {
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE)) {
                if (currentScreen == SCREEN_ANIMASI) {
                    StopMusicStream(bgMusic);
                    if (isMenuBgmOn) PlayMusicStream(menuMusic);
                }
                currentScreen = SCREEN_MENU;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        GameScreen oldScreen = currentScreen;

        switch (currentScreen) 
        {
            case SCREEN_MENU:
                if (isMenuBgmOn != prevMenuBgmOn) {
                    if (isMenuBgmOn) {
                        PlayMusicStream(menuMusic);
                    } else {
                        StopMusicStream(menuMusic);
                    }
                    prevMenuBgmOn = isMenuBgmOn;
                }

                currentScreen = UpdateDrawMenuScreen(customFont, bgMenu, screenWidth, screenHeight, showOutline, currentScreen, &isMenuBgmOn);
                if (oldScreen == SCREEN_MENU && currentScreen == SCREEN_ANIMASI) {
                    animStartTime = (float)GetTime(); 
                    StopMusicStream(menuMusic);
                    PlayMusicStream(bgMusic);
                    if (!isAnimBgmOn) PauseMusicStream(bgMusic);
                }
                if ((int)currentScreen == -1) goto EXIT_GAME; 
                break;
                
            case SCREEN_ANIMASI:
                if (isAnimBgmOn != prevAnimBgmOn) {
                    if (isAnimBgmOn) {
                        ResumeMusicStream(bgMusic);
                    } else {
                        PauseMusicStream(bgMusic);
                    }
                    prevAnimBgmOn = isAnimBgmOn;
                }

                if (isAnimBgmOn) {
                    UpdateMusicStream(bgMusic);
                }
                
                float animTime = (float)GetTime() - animStartTime; 
                DrawAnimasiScene(screenWidth, screenHeight, animTime, &isAnimBgmOn, customFont);
                break;
                
            case SCREEN_POLA:
                ClearBackground(BLACK); 
                
                Rectangle sourceRecPola = { 0.0f, 0.0f, (float)bgPola.width, (float)bgPola.height };
                Rectangle destRecPola = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
                DrawTexturePro(bgPola, sourceRecPola, destRecPola, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
                
                if (IsKeyPressed(KEY_RIGHT)) { polaViewMode++; if (polaViewMode > 8) polaViewMode = 1; }
                if (IsKeyPressed(KEY_LEFT)) { polaViewMode--; if (polaViewMode < 1) polaViewMode = 8; }

                const char *polaTitle = "";
                if (polaViewMode == 1) polaTitle = "Bulan";
                else if (polaViewMode == 2) polaTitle = "Bulan Rinne Sharingan";
                else if (polaViewMode == 3) polaTitle = "Pohon Shinju";
                else if (polaViewMode == 4) polaTitle = "Gunung";
                else if (polaViewMode == 5) polaTitle = "Uchiha Madara";
                else if (polaViewMode == 6) polaTitle = "Naruto (Kurama Mode)";
                else if (polaViewMode == 7) polaTitle = "Sasuke (Perfect Susanoo)";
                else if (polaViewMode == 8) polaTitle = "Awan, Bintang & Debu";

                float titleFontSize = 60.0f;
                Vector2 titleSize = MeasureTextEx(customFont, polaTitle, titleFontSize, 2);
                DrawTextEx(customFont, polaTitle, (Vector2){ (screenWidth - titleSize.x) / 2.0f, 80.0f }, titleFontSize, 2, WHITE);

                int center_x = screenWidth / 2;
                int center_y = screenHeight / 2;

                if (polaViewMode == 1) {
                    DrawBulanBiasa(center_x, center_y, 180, (float)GetTime() * 0.5f, fabsf(sinf(GetTime() * 3.0f)), 1.0f);
                } 
                else if (polaViewMode == 2) {
                    DrawBulanMerah(center_x, center_y, 180, (float)GetTime() * 0.5f, fabsf(sinf(GetTime() * 3.0f)), 1.0f);
                }
                else if (polaViewMode == 3) {
                    float treeScale = 0.8f * ((float)screenHeight / 720.0f);
                    DrawPohonShinju(center_x, screenHeight - 50, treeScale);
                }
                else if (polaViewMode == 4) {
                    DrawGunungForeground(screenWidth, screenHeight);
                }
                else if (polaViewMode == 5) {
                    DrawMadaraRikudou(center_x, center_y, 2.0f, (float)GetTime());
                }
                else if (polaViewMode == 6) {
                    DrawNarutoKurama(center_x, center_y, 2.0f, (float)GetTime());
                }
                else if (polaViewMode == 7) {
                    DrawSasukeSusanoo(center_x, center_y, 2.0f, (float)GetTime());
                }
                else if (polaViewMode == 8) {
                    UpdateAndDrawBintangAwan(screenWidth, screenHeight);
                    UpdateAndDrawDebu(screenWidth, screenHeight);
                }

                Color btnBg = (Color){20, 30, 50, 255};
                Color btnHover = MAROON;
                
                float btnSize = 50.0f;
                Rectangle leftBtn = { 30.0f, (screenHeight - btnSize) / 2.0f, btnSize, btnSize };
                Rectangle rightBtn = { screenWidth - 30.0f - btnSize, (screenHeight - btnSize) / 2.0f, btnSize, btnSize };

                Vector2 mousePos = GetMousePosition();
                bool leftHovered = CheckCollisionPointRec(mousePos, leftBtn);
                bool rightHovered = CheckCollisionPointRec(mousePos, rightBtn);

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (leftHovered) {
                        polaViewMode--;
                        if (polaViewMode < 1) polaViewMode = 8;
                    }
                    if (rightHovered) {
                        polaViewMode++;
                        if (polaViewMode > 8) polaViewMode = 1;
                    }
                }

                DrawRectangleRec(leftBtn, leftHovered ? btnHover : btnBg);
                DrawRectangleLinesEx(leftBtn, 2, WHITE);
                DrawTextEx(customFont, "<", (Vector2){ leftBtn.x + 15, leftBtn.y + 10 }, 30, 1, WHITE);

                DrawRectangleRec(rightBtn, rightHovered ? btnHover : btnBg);
                DrawRectangleLinesEx(rightBtn, 2, WHITE);
                DrawTextEx(customFont, ">", (Vector2){ rightBtn.x + 15, rightBtn.y + 10 }, 30, 1, WHITE);

                break;
                
            case SCREEN_ABOUT:
                DrawAboutScreen(customFont, bgAbout, screenWidth, screenHeight);
                break;
        }

        EndDrawing();
    }

EXIT_GAME: 
    UnloadMusicStream(bgMusic);
    UnloadMusicStream(menuMusic);
    CloseAudioDevice();
    UnloadTexture(bgMenu);
    UnloadTexture(bgAbout); 
    UnloadTexture(bgPola); 
    UnloadFont(customFont); 
    CloseWindow();

    return 0;
}