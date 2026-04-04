#include "menu.h"
#include <stdbool.h>

typedef struct {
    Rectangle rect;
    const char *text;
    Color color;
    Color hoverColor;
    Color textColor;
} Button;

GameScreen UpdateDrawMenuScreen(Font font, Texture2D bg, int screenWidth, int screenHeight, bool showOutline, GameScreen currentScreen, bool *isMenuBgmOn)
{
    Vector2 mousePoint = GetMousePosition();
    bool btnClicked = false;
    int clickedBtnIndex = -1;

    float btnWidth = 250.0f;
    float btnHeight = 50.0f;
    float btnSpacing = 20.0f;

    float startX = (screenWidth - btnWidth) / 2.0f;
    float startY = screenHeight * 0.45f;

    Button buttons[5] = {
        {{startX, startY, btnWidth, btnHeight}, "ANIMASI", {20, 30, 50, 255}, MAROON, RED},
        {{startX, startY + (btnHeight + btnSpacing), btnWidth, btnHeight}, "POLA", {20, 30, 50, 255}, MAROON, RED},
        {{startX, startY + (btnHeight + btnSpacing)*2, btnWidth, btnHeight}, "ABOUT", {20, 30, 50, 255}, MAROON, RED},
        {{startX, startY + (btnHeight + btnSpacing)*3, btnWidth, btnHeight}, *isMenuBgmOn ? "BGM OFF" : "BGM ON", {20, 30, 50, 255}, MAROON, RED},
        {{startX, startY + (btnHeight + btnSpacing)*4, btnWidth, btnHeight}, "EXIT", {20, 30, 50, 255}, MAROON, RED}
    };

    for (int i = 0; i < 5; i++) {
        if (CheckCollisionPointRec(mousePoint, buttons[i].rect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                clickedBtnIndex = i;
                btnClicked = true;
            }
        }
    }

    if (btnClicked) {
        if (clickedBtnIndex == 0) return SCREEN_ANIMASI;
        if (clickedBtnIndex == 1) return SCREEN_POLA;
        if (clickedBtnIndex == 2) return SCREEN_ABOUT;
        if (clickedBtnIndex == 3) {
            *isMenuBgmOn = !(*isMenuBgmOn);
            return currentScreen;
        }
        if (clickedBtnIndex == 4) return -1; 
    }

    Rectangle sourceRec = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(bg, sourceRec, destRec, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

    float titleSize = screenWidth / 22.0f;
    float fontSpacing = 2.0f;
    const char *titleLine1 = "SIMULASI";
    const char *titleLine2 = "MUGEN TSUKUYOMI";

    Vector2 size1 = MeasureTextEx(font, titleLine1, titleSize, fontSpacing);
    Vector2 size2 = MeasureTextEx(font, titleLine2, titleSize, fontSpacing);

    float lineSpacing = titleSize / 4.0f;
    float titleCenterY = screenHeight * 0.25f;
    float titleY1 = titleCenterY - size1.y + (lineSpacing / 2.0f);
    float titleY2 = titleCenterY + (lineSpacing / 2.0f);
    float titleX1 = (screenWidth - size1.x) / 2.0f;
    float titleX2 = (screenWidth - size2.x) / 2.0f;

    DrawTextEx(font, titleLine1, (Vector2){titleX1 + 4, titleY1 + 4}, titleSize, fontSpacing, BLACK);
    DrawTextEx(font, titleLine1, (Vector2){titleX1, titleY1}, titleSize, fontSpacing, RED);

    DrawTextEx(font, titleLine2, (Vector2){titleX2 + 4, titleY2 + 4}, titleSize, fontSpacing, BLACK);
    DrawTextEx(font, titleLine2, (Vector2){titleX2, titleY2}, titleSize, fontSpacing, RED);

    for (int i = 0; i < 5; i++) {
        bool isHovered = CheckCollisionPointRec(mousePoint, buttons[i].rect);
        bool isPressed = isHovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        Rectangle drawRect = buttons[i].rect;
        if (isPressed) {
            drawRect.x += 2; drawRect.y += 2;
            drawRect.width -= 4; drawRect.height -= 4;
        }

        if (showOutline) {
            DrawRectangleLinesEx(drawRect, 2.0f, isHovered ? buttons[i].textColor : WHITE);
        } else {
            DrawRectangleRec(drawRect, isHovered ? buttons[i].hoverColor : buttons[i].color);
            DrawRectangleLinesEx(drawRect, 2.0f, buttons[i].textColor);
        }

        float btnFontSize = 26.0f;
        float btnFontSpacing = 2.0f;
        Vector2 textSize = MeasureTextEx(font, buttons[i].text, btnFontSize, btnFontSpacing);
        Vector2 textPos = { drawRect.x + (drawRect.width - textSize.x) / 2.0f, drawRect.y + (drawRect.height - textSize.y) / 2.0f };
        DrawTextEx(font, buttons[i].text, textPos, btnFontSize, btnFontSpacing, buttons[i].textColor);
    }

    DrawTextEx(font, "Tekan 'O' untuk toggle Outline UI", (Vector2){10, 10}, 20.0f, 1.0f, LIGHTGRAY);

    return currentScreen; 
}