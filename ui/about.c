#include "about.h"

void DrawAboutScreen(Font font, Texture2D bg, int screenWidth, int screenHeight)
{
    Rectangle sourceRec = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(bg, sourceRec, destRec, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

    const char *descText = 
        "Mugen Tsukuyomi/Tsukuyomi Tak Terbatas adalah sebuah\n"
        "teknik Rencana Mata Bulan yang dimana pengguna dapat\n"
        "menciptakan ilusi abadi di bawah kendali mereka.\n"
        "Ilusi ini akan mengikuti keinginan orang yang terkena.\n"
        "Uchiha Madara menggunakan kekuatan Juubi dan Rinne\n"
        "Sharingan untuk menciptakan ilusi ini.\n\n"

        "Created By:\n\n"
        "Adi Rafi Chaerufarizki - 241524001\n"
        "2A - D4 Teknik Informatika\n\n"

        "Credit:\n\n"
        "BGM Animasi = Waltz of Wind and Fire - Yasuharu Takanashi\n"
        "BGM Menu = Tsuki no Ookisa (Off Vocal Version) - Nogizaka46\n";
     
    float descSize = 23.0f; 
    float descSpacing = 3.0f;
    Vector2 textSize = MeasureTextEx(font, descText, descSize, descSpacing);

    float panelWidth = textSize.x + 100.0f; 
    float panelHeight = textSize.y + 160.0f; 
    
    if (panelWidth > screenWidth * 0.95f) panelWidth = screenWidth * 0.95f;
    if (panelHeight > screenHeight * 0.95f) panelHeight = screenHeight * 0.95f;

    float panelX = (screenWidth - panelWidth) / 2.0f;
    float panelY = (screenHeight - panelHeight) / 2.0f - 20; 

    DrawRectangleLinesEx((Rectangle){panelX, panelY, panelWidth, panelHeight}, 5.0f, MAROON);

    const char *aboutTitle = "ABOUT";
    float titleSize = 48.0f; 
    Vector2 titleMeasure = MeasureTextEx(font, aboutTitle, titleSize, 2.0f);
    Vector2 titlePos = { panelX + (panelWidth - titleMeasure.x) / 2.0f, panelY + 30.0f };
    
    DrawTextEx(font, aboutTitle, (Vector2){titlePos.x + 3, titlePos.y + 3}, titleSize, 2.0f, BLACK);
    DrawTextEx(font, aboutTitle, titlePos, titleSize, 2.0f, RED);

    DrawLineEx((Vector2){panelX + 40, titlePos.y + 60}, (Vector2){panelX + panelWidth - 40, titlePos.y + 60}, 2.0f, DARKGRAY);

    Vector2 descPos = { panelX + 50.0f, titlePos.y + 100.0f };
    DrawTextEx(font, descText, (Vector2){descPos.x + 2, descPos.y + 2}, descSize, descSpacing, BLACK);
    DrawTextEx(font, descText, descPos, descSize, descSpacing, LIGHTGRAY);

    const char *backText = "[ BACKSPACE UNTUK KEMBALI ]";
    float backSize = 20.0f; 
    Vector2 backMeasure = MeasureTextEx(font, backText, backSize, 2.0f);
    Vector2 backPos = { panelX + (panelWidth - backMeasure.x) / 2.0f, panelY + panelHeight + 40.0f };
    
    DrawTextEx(font, backText, (Vector2){backPos.x + 2, backPos.y + 2}, backSize, 2.0f, BLACK); 
    DrawTextEx(font, backText, backPos, backSize, 2.0f, GRAY); 
}