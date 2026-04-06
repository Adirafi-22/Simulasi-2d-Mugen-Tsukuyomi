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
    bool isDraggingSpeed = false;

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

        if (currentScreen == SCREEN_ANIMASI || currentScreen == SCREEN_POLA) {
            g_accumulatedTime += GetFrameTime() * g_animSpeed;
            if (g_jtkSmokeTimer > 0.0f) {
                g_jtkSmokeTimer -= GetFrameTime() * g_animSpeed;
                if (g_jtkSmokeTimer < 0.0f) g_jtkSmokeTimer = 0.0f;
            }
        }

        if (IsKeyPressed(KEY_O)) {
            g_isWireframe = !g_isWireframe;
        }

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
                if (oldScreen == SCREEN_MENU && currentScreen != SCREEN_MENU) {
                    g_isWireframe = false;
                    g_animSpeed = 1.0f;
                    SetMusicPitch(bgMusic, 1.0f);
                    g_sharinganDir = 1.0f;
                    
                    if (currentScreen == SCREEN_ANIMASI) {
                        g_accumulatedTime = 0.0f;
                        StopMusicStream(menuMusic);
                        PlayMusicStream(bgMusic);
                        if (!isAnimBgmOn) PauseMusicStream(bgMusic);
                    }
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
                
                DrawAnimasiScene(screenWidth, screenHeight, g_accumulatedTime, &isAnimBgmOn, customFont);
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
                    DrawBulanBiasa(center_x, center_y, 180, g_accumulatedTime * 0.5f, fabsf(sinf(g_accumulatedTime * 3.0f)), 1.0f);
                } 
                else if (polaViewMode == 2) {
                    DrawBulanMerah(center_x, center_y, 180, g_accumulatedTime * 0.5f, fabsf(sinf(g_accumulatedTime * 3.0f)), 1.0f);
                }
                else if (polaViewMode == 3) {
                    float treeScale = 0.8f * ((float)screenHeight / 720.0f);
                    DrawPohonShinju(center_x, screenHeight - 50, treeScale);
                }
                else if (polaViewMode == 4) {
                    DrawGunungForeground(screenWidth, screenHeight);
                }
                else if (polaViewMode == 5) {
                    DrawMadaraRikudou(center_x, center_y, 2.0f, g_accumulatedTime);
                }
                else if (polaViewMode == 6) {
                    DrawNarutoKurama(center_x, center_y, 2.0f, g_accumulatedTime);
                }
                else if (polaViewMode == 7) {
                    DrawSasukeSusanoo(center_x, center_y, 2.0f, g_accumulatedTime);
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

        // Draw overlay Challenge controls for ANIMASI & POLA
        if (currentScreen == SCREEN_ANIMASI || currentScreen == SCREEN_POLA) {
            int btnY = 20;
            
            bool showSpeed = (currentScreen == SCREEN_ANIMASI);
            bool showDir = (currentScreen == SCREEN_ANIMASI) || (currentScreen == SCREEN_POLA && (polaViewMode == 1 || polaViewMode == 2));
            
            Rectangle speedPanel = { 20, btnY, 180, 50 };
            Rectangle track = { 30, btnY + 30, 160, 6 };
            Rectangle sliderHitbox = { track.x - 10, track.y - 15, track.width + 20, track.height + 30 };
            
            Rectangle dirBtn;
            Rectangle wireBtn;
            
            if (currentScreen == SCREEN_ANIMASI) {
                dirBtn = (Rectangle){ 220, btnY, 350, 50 };
                wireBtn = (Rectangle){ 590, btnY, 200, 50 };
            } else {
                int currentX = 20;
                if (showDir) {
                    dirBtn = (Rectangle){ currentX, btnY, 350, 50 };
                    currentX += 370;
                }
                wireBtn = (Rectangle){ currentX, btnY, 200, 50 };
            }
            
            Rectangle jtkBtn = { 20, btnY + 70, 200, 50 };
            
            Vector2 mousePos = GetMousePosition();
            
            // Speed Logic
            if (showSpeed) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, sliderHitbox)) {
                    isDraggingSpeed = true;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    isDraggingSpeed = false;
                }
                if (isDraggingSpeed) {
                    float normalizedX = (mousePos.x - track.x) / track.width;
                    if (normalizedX < 0.0f) normalizedX = 0.0f;
                    if (normalizedX > 1.0f) normalizedX = 1.0f;
                    g_animSpeed = normalizedX * 5.0f;
                    SetMusicPitch(bgMusic, (g_animSpeed < 0.05f) ? 0.05f : g_animSpeed);
                }
            }
            
            // Dir Logic
            if (showDir) {
                if (CheckCollisionPointRec(mousePos, dirBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    g_sharinganDir = (g_sharinganDir == 1.0f) ? -1.0f : 1.0f;
                }
            }
            
            // Wireframe Logic
            if (CheckCollisionPointRec(mousePos, wireBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                g_isWireframe = !g_isWireframe;
            }
            
            // JTK btn Logic
            if (currentScreen == SCREEN_ANIMASI) {
                if (CheckCollisionPointRec(mousePos, jtkBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    g_showJTK = !g_showJTK;
                    g_jtkSmokeTimer = 1.0f; // Start smoke animation (1 second)
                }
            }

            Color btnBgPanel = (Color){20, 30, 50, 200};
            Color btnHoverPanel = MAROON;
            
            if (showSpeed) {
                DrawRectangleRec(speedPanel, btnBgPanel);
                DrawRectangleLinesEx(speedPanel, 2, WHITE);
                DrawTextEx(customFont, TextFormat("SPEED: %.1fx", g_animSpeed), (Vector2){30, btnY + 5}, 20, 1, WHITE);
                
                Rectangle knob = { track.x + (g_animSpeed / 5.0f) * track.width - 5, btnY + 22, 10, 22 };
                if (knob.x < track.x) knob.x = track.x;
                if (knob.x > track.x + track.width - knob.width) knob.x = track.x + track.width - knob.width;
                
                DrawRectangleRec(track, GRAY);
                DrawRectangleRec(knob, isDraggingSpeed ? MAROON : WHITE);
            }
            
            if (showDir) {
                DrawRectangleRec(dirBtn, CheckCollisionPointRec(mousePos, dirBtn) ? btnHoverPanel : btnBgPanel);
                DrawRectangleLinesEx(dirBtn, 2, WHITE);
                DrawTextEx(customFont, g_sharinganDir == 1.0f ? "ARAH BULAN: SEARAH" : "ARAH BULAN: BERLAWANAN", (Vector2){dirBtn.x + 10, dirBtn.y + 15}, 24, 1, WHITE);
            }
            
            // Wireframe btn
            DrawRectangleRec(wireBtn, CheckCollisionPointRec(mousePos, wireBtn) ? btnHoverPanel : btnBgPanel);
            DrawRectangleLinesEx(wireBtn, 2, WHITE);
            DrawTextEx(customFont, g_isWireframe ? "WIREFRAME: ON" : "WIREFRAME: OFF", (Vector2){wireBtn.x + 10, wireBtn.y + 15}, 24, 1, WHITE);
            
            // JTK btn
            if (currentScreen == SCREEN_ANIMASI) {
                DrawRectangleRec(jtkBtn, CheckCollisionPointRec(mousePos, jtkBtn) ? btnHoverPanel : btnBgPanel);
                DrawRectangleLinesEx(jtkBtn, 2, WHITE);
                DrawTextEx(customFont, g_showJTK ? "HILANGKAN" : "KAGEBUNSHIN", (Vector2){jtkBtn.x + 10, jtkBtn.y + 15}, 24, 1, WHITE);
            }
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