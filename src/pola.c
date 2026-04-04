#include "pola.h"
#include "algoritma.h"
#include <math.h>

Color LerpColor(Color a, Color b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (Color){
        (unsigned char)(a.r + t * (b.r - a.r)),
        (unsigned char)(a.g + t * (b.g - a.g)),
        (unsigned char)(a.b + t * (b.b - a.b)),
        255
    };
}

// Tomoe Penuh
static void DrawTomoeFilled(float cx, float cy, float scale, float rotation, Color color) {
    float rBula = 20.0f * scale; // Kepala Tomoe
    MidcircleFilled((int)cx, (int)cy, (int)rBula, color);

    // Titik kontrol Kurva Bezier
    float px0 = 14 * scale, py0 = 0;
    float px1 = 14 * scale, py1 = 20 * scale;
    float px2 = 4 * scale,  py2 = 35 * scale;
    float px3 = -10 * scale, py3 = 45 * scale;

    // Transformasi Rotasi
    RotatePoint(&px0, &py0, 0, 0, rotation);
    RotatePoint(&px1, &py1, 0, 0, rotation);
    RotatePoint(&px2, &py2, 0, 0, rotation);
    RotatePoint(&px3, &py3, 0, 0, rotation);

    // Mencetak lingkaran yang makin mengecil di sepanjang kurva
    for (float t = 0.0f; t <= 1.0f; t += 0.02f) {
        float u = 1.0f - t;
        float tx = u*u*u*px0 + 3*u*u*t*px1 + 3*u*t*t*px2 + t*t*t*px3;
        float ty = u*u*u*py0 + 3*u*u*t*py1 + 3*u*t*t*py2 + t*t*t*py3;
        
        float stampRadius = (1.0f - t) * (14.0f * scale); 
        if(stampRadius > 0.5f) {
            MidcircleFilled((int)(cx + tx), (int)(cy + ty), (int)stampRadius, color);
        }
    }
}

static RenderTexture2D bgCache = {0};
static int lastBgWidth = 0;
static int lastBgHeight = 0;

void DrawBackgroundGradient(int width, int height) {
    if (bgCache.id == 0 || lastBgWidth != width || lastBgHeight != height) {
        if (bgCache.id != 0) UnloadRenderTexture(bgCache);
        bgCache = LoadRenderTexture(width, height);
        BeginTextureMode(bgCache);
        ClearBackground(BLACK);
        
        Color topColor = (Color){2, 0, 20, 255};     // Gelap Hitam/Navy
        Color bottomColor = (Color){58, 0, 0, 255};  // Merah Maroon

        // Fill menggunakan garis horizontal Bresenham
        for(int y = 0; y < height; y++) {
            float t = (float)y / height;
            Color c = LerpColor(topColor, bottomColor, t);
            BresenhamLine(0, y, width, y, c);
        }
        EndTextureMode();
        lastBgWidth = width;
        lastBgHeight = height;
    }

    // Gambar tekstur cache yang sudah jadi (flip Y karena render texture OpenGL terbalik)
    DrawTextureRec(bgCache.texture, (Rectangle){ 0, 0, (float)width, -(float)height }, (Vector2){ 0, 0 }, WHITE);
}

// =========================================================
// BULAN
// =========================================================
static RenderTexture2D bulanBiasaCache = {0};

void DrawBulanBiasa(int cx, int cy, int radius, float rotasi, float glowIntensitas, float alphaFading) {
    Color blueMoon = (Color){224, 247, 250, 255};
    Color craterCol = (Color){200, 230, 240, 255}; 
    
    // Efek Glow Biru (Glow digambar real-time) Fast Render
    int glowRadius = radius + 5 + (int)(glowIntensitas * 15.0f);
    DrawCircleGradient(cx, cy, glowRadius, (Color){150, 200, 255, (unsigned char)(40 * alphaFading)}, BLANK);

    if (bulanBiasaCache.id == 0) {
        int tempRadius = 200;
        bulanBiasaCache = LoadRenderTexture(tempRadius * 2, tempRadius * 2);
        BeginTextureMode(bulanBiasaCache);
        ClearBackground(BLANK);
        
        int tcx = tempRadius, tcy = tempRadius;
        MidcircleFilled(tcx, tcy, tempRadius, blueMoon);
        
        // Kawah TANPA ROTASI saat pembuatan cache
        float factor = 200.0f / 180.0f;
        float c1x = -30 * factor, c1y = -40 * factor;
        float c2x = 40 * factor,  c2y = -20 * factor;
        float c3x = -10 * factor, c3y = 50 * factor;
        float c4x = 50 * factor,  c4y = 40 * factor;

        MidcircleFilled(tcx + (int)c1x, tcy + (int)c1y, (int)(15 * factor), craterCol);
        MidcircleFilled(tcx + (int)c2x, tcy + (int)c2y, (int)(25 * factor), craterCol);
        MidcircleFilled(tcx + (int)c3x, tcy + (int)c3y, (int)(20 * factor), craterCol);
        MidpointEllipseFilled(tcx + (int)c4x, tcy + (int)c4y, (int)(10 * factor), (int)(9 * factor), craterCol);
        
        EndTextureMode();
    }
    
    // Gambar cache bulan biasa
    float scale = (float)radius / 200.0f;
    Rectangle source = { 0, 0, (float)bulanBiasaCache.texture.width, -(float)bulanBiasaCache.texture.height };
    Rectangle dest = { (float)cx, (float)cy, (float)bulanBiasaCache.texture.width * scale, (float)bulanBiasaCache.texture.height * scale };
    Vector2 origin = { (float)bulanBiasaCache.texture.width * scale / 2.0f, (float)bulanBiasaCache.texture.height * scale / 2.0f };
    
    Color tint = (Color){255, 255, 255, (unsigned char)(255 * alphaFading)};
    DrawTexturePro(bulanBiasaCache.texture, source, dest, origin, rotasi * RAD2DEG, tint);
}

// =========================================================
// BULAN RINNE SHARINGAN
// =========================================================
static RenderTexture2D bulanMerahCache = {0};

void DrawBulanMerah(int cx, int cy, int radius, float rotasi, float glowIntensitas, float alphaFading) {
    Color redMoon = (Color){183, 0, 0, 255};
    Color blackCol = (Color){0, 0, 0, 255};

    int glowRadius = radius + 5 + (int)(glowIntensitas * 15.0f);
    DrawCircleGradient(cx, cy, glowRadius, (Color){255, 0, 0, (unsigned char)(40 * alphaFading)}, BLANK);

    if (bulanMerahCache.id == 0) {
        int tempRadius = 200;
        bulanMerahCache = LoadRenderTexture(tempRadius * 2, tempRadius * 2);
        BeginTextureMode(bulanMerahCache);
        ClearBackground(BLANK);
        
        int tcx = tempRadius, tcy = tempRadius;
        DrawCircle(tcx, tcy, tempRadius, redMoon);
        
        float factor = 200.0f / 180.0f;
        float uniformScale = 0.65f * factor;

        // Titik pusat (Pupil) full hitam
        DrawCircle(tcx, tcy, (int)(25.0f * uniformScale), blackCol);

        int ringRadii[3] = {(int)(tempRadius*0.35f), (int)(tempRadius*0.62f), (int)(tempRadius*0.85f)};

        for (int i = 0; i < 3; i++) {
            int r = ringRadii[i];
            DrawCircleLines(tcx, tcy, r, blackCol);

            for (int j = 0; j < 3; j++) {
                float angleOffset = (i % 2 == 1) ? PI : 0;
                float angle = -PI / 2.0f + (j * PI * 2.0f / 3.0f) + angleOffset;
                
                float tx = tcx + cosf(angle) * r;
                float ty = tcy + sinf(angle) * r;
                float tomoeRot = angle - PI / 2.0f; 
                
                DrawTomoeFilled(tx, ty, uniformScale, tomoeRot, blackCol);
            }
        }
        EndTextureMode();
    }
    
    Color tint = (Color){255, 255, 255, (unsigned char)(255 * alphaFading)};
    
    float scale = (float)radius / 200.0f;
    Rectangle source = { 0, 0, (float)bulanMerahCache.texture.width, -(float)bulanMerahCache.texture.height };
    Rectangle dest = { (float)cx, (float)cy, (float)bulanMerahCache.texture.width * scale, (float)bulanMerahCache.texture.height * scale };
    Vector2 origin = { (float)bulanMerahCache.texture.width * scale / 2.0f, (float)bulanMerahCache.texture.height * scale / 2.0f };
    
    DrawTexturePro(bulanMerahCache.texture, source, dest, origin, rotasi * RAD2DEG, tint);
}

// =========================================================
// POHON SHINJU
// =========================================================
static RenderTexture2D pohonCache = {0};

void DrawPohonShinju(int baseX, int baseY, float scale) {
    if (pohonCache.id == 0) {
        pohonCache = LoadRenderTexture(1000, 900);
        BeginTextureMode(pohonCache);
        ClearBackground(BLANK);
        
        int tempBaseX = 500;
        int tempBaseY = 800; 
        float tempScale = 1.0f;
        Color darkWood = (Color){85, 70, 60, 255};   
        Color midWood  = (Color){120, 105, 95, 255}; 

        
        int height = (int)(600 * tempScale);
        int topWidth = (int)(25 * tempScale);
        int bottomWidth = (int)(110 * tempScale); 
        int topY = tempBaseY - height;

        int rootHeights[3] = { height / 3,   height / 5,   height / 10 }; 
        int rootSpreads[3] = { 280,          400,          500 };         
        int rootThick[3]   = { 25,           30,           40 };          
        int rootDrops[3]   = { 0,            30,           60 };          // Seberapa dalam akar memanjang ke bawah

        for (int r = 0; r < 3; r++) {
            int rootStartY = tempBaseY - rootHeights[r];
            float t_root = (float)(rootHeights[r]) / height; 
            int startW = bottomWidth - (int)(t_root * (bottomWidth - topWidth));

            for (int w = 0; w < rootThick[r] * tempScale; w++) {
                int endOffsetY = w / 2; 

                // Akar Kiri
                DrawBezierCubic(tempBaseX - startW + (w/3), rootStartY + w*1.5f,
                                tempBaseX - (int)(150*tempScale), rootStartY + (int)(40*tempScale) + w,
                                tempBaseX - (int)(rootSpreads[r]*0.7f*tempScale), tempBaseY - (int)(10*tempScale) + endOffsetY,
                                tempBaseX - (int)(rootSpreads[r]*tempScale), tempBaseY + (int)(rootDrops[r]*tempScale), darkWood);
                // Akar Kanan
                DrawBezierCubic(tempBaseX + startW - (w/3), rootStartY + w*1.5f,
                                tempBaseX + (int)(150*tempScale), rootStartY + (int)(40*tempScale) + w,
                                tempBaseX + (int)(rootSpreads[r]*0.7f*tempScale), tempBaseY - (int)(10*tempScale) + endOffsetY,
                                tempBaseX + (int)(rootSpreads[r]*tempScale), tempBaseY + (int)(rootDrops[r]*tempScale), darkWood);
            }
        }

        // Batang
        for (int y = 0; y <= height; y++) {
            float t = (float)y / height;
            int currentWidth = bottomWidth - (int)(t * (bottomWidth - topWidth));
            DrawLine(tempBaseX - currentWidth, tempBaseY - y, tempBaseX + currentWidth, tempBaseY - y, midWood); 
        }
        for (int y = 0; y <= height; y++) {
            float t = (float)y / height;
            int currentWidth = bottomWidth - (int)(t * (bottomWidth - topWidth));
            DrawLine(tempBaseX - currentWidth, tempBaseY - y, tempBaseX - (int)(currentWidth * 0.7f), tempBaseY - y, darkWood);
            DrawLine(tempBaseX + (int)(currentWidth * 0.7f), tempBaseY - y, tempBaseX + currentWidth, tempBaseY - y, darkWood);
        }


        // Tekstur Serat Melilit
        for (int i = -1; i <= 1; i++) {
            int offsetX = i * (bottomWidth/2);
            DrawBezierCubic(tempBaseX + offsetX, tempBaseY,
                            tempBaseX - (int)(60*tempScale) + offsetX, tempBaseY - (int)(200*tempScale),
                            tempBaseX + (int)(60*tempScale) + offsetX, tempBaseY - (int)(400*tempScale),
                            tempBaseX, topY, darkWood);
                            
            DrawBezierCubic(tempBaseX - offsetX, tempBaseY,
                            tempBaseX + (int)(60*tempScale) - offsetX, tempBaseY - (int)(200*tempScale),
                            tempBaseX - (int)(60*tempScale) - offsetX, tempBaseY - (int)(400*tempScale),
                            tempBaseX, topY, darkWood);
        }

        // Mahkota Tajam (Bunga)
        Color spikeCol = (Color){45, 35, 33, 255};
        Bres_ThickLine(tempBaseX, topY, tempBaseX, topY - (int)(80*tempScale), 12, spikeCol); // Pucuk Tengah
        Bres_ThickLine(tempBaseX, topY, tempBaseX - (int)(50*tempScale), topY - (int)(60*tempScale), 10, spikeCol); // Kiri Dalam
        Bres_ThickLine(tempBaseX, topY, tempBaseX + (int)(50*tempScale), topY - (int)(60*tempScale), 10, spikeCol); // Kanan Dalam
        Bres_ThickLine(tempBaseX, topY + (int)(15*tempScale), tempBaseX - (int)(70*tempScale), topY - (int)(20*tempScale), 8, spikeCol); // Kiri Luar
        Bres_ThickLine(tempBaseX, topY + (int)(15*tempScale), tempBaseX + (int)(70*tempScale), topY - (int)(20*tempScale), 8, spikeCol); // Kanan Luar
        
        EndTextureMode();
    }
    
    Rectangle source = { 0, 0, (float)pohonCache.texture.width, -(float)pohonCache.texture.height };
    Rectangle dest = { (float)baseX, (float)baseY, (float)pohonCache.texture.width * scale, (float)pohonCache.texture.height * scale };
    Vector2 origin = { 500.0f * scale, 800.0f * scale };
    
    DrawTexturePro(pohonCache.texture, source, dest, origin, 0.0f, WHITE);
}

// =========================================================
// GUNUNG
// =========================================================
static RenderTexture2D gunungCache = {0};
static int lastGunungWidth = 0;
static int lastGunungHeight = 0;

void DrawGunungForeground(int screenWidth, int screenHeight) {
    if (gunungCache.id == 0 || lastGunungWidth != screenWidth || lastGunungHeight != screenHeight) {
        if (gunungCache.id != 0) UnloadRenderTexture(gunungCache);
        gunungCache = LoadRenderTexture(screenWidth, screenHeight);
        BeginTextureMode(gunungCache);
        ClearBackground(BLANK); // Transparan agar bisa di-layer
        
        Color mountainCol = (Color){15, 20, 40, 255}; // Navy Gelap Estetik
        int baseY = screenHeight;

        float scaleX = (float)screenWidth / 1280.0f;
        float scaleY = (float)screenHeight / 720.0f;
        int peaksX[] = {
            0,
            (int)(150 * scaleX),
            (int)(300 * scaleX),
            (int)(450 * scaleX),
            (int)(650 * scaleX),
            (int)(850 * scaleX),
            (int)(1050 * scaleX),
            screenWidth
        };
        int peaksY[] = {
            baseY-(int)(50*scaleY), baseY-(int)(130*scaleY), baseY-(int)(80*scaleY), 
            baseY-(int)(170*scaleY), baseY-(int)(100*scaleY), baseY-(int)(220*scaleY), 
            baseY-(int)(90*scaleY), baseY-(int)(50*scaleY)
        };

        for (int i = 0; i < 7; i++) {
            int x1 = peaksX[i], y1 = peaksY[i];
            int x2 = peaksX[i+1], y2 = peaksY[i+1];
            
            for(int x = x1; x <= x2; x++) {
                float t = (float)(x - x1) / (x2 - x1);
                int y = y1 + (int)(t * (y2 - y1)); 
                DrawLine(x, y, x, screenHeight, mountainCol); // Changed from BresenhamLine
            }
            DrawLineEx((Vector2){(float)x1, (float)y1}, (Vector2){(float)x2, (float)y2}, 3, BLACK); // Changed from Bres_ThickLine
        }
        EndTextureMode();
        lastGunungWidth = screenWidth;
        lastGunungHeight = screenHeight;
    }
    
    DrawTextureRec(gunungCache.texture, (Rectangle){ 0, 0, (float)screenWidth, -(float)screenHeight }, (Vector2){ 0, 0 }, WHITE);
}

// =========================================================
// ANIMASI MUGEN TSUKUYOMI
// =========================================================
void DrawBulanAnimasi(int cx, int cy, int radius, float elapsedTime) {
    
    float rotasiGlobal = elapsedTime * 0.2f; 
    // Double-sine wave interpolation untuk pendaran (glow) bulan yang lebih organik dan smooth
    float wave1 = sinf(elapsedTime * 1.5f);
    float wave2 = sinf(elapsedTime * 2.7f) * 0.3f;
    float pulse = fabsf(wave1 + wave2); 

    // FASE 1: BULAN BIASA 
    if (elapsedTime <= 5.5f) {
        DrawBulanBiasa(cx, cy, radius, rotasiGlobal, pulse, 1.0f);
        // Pre-cache memori agar saat transisi pertama kali tidak freeze
        if (elapsedTime == 0.0f) DrawBulanMerah(100, 100, radius, 0.0f, 0.0f, 0.0f);
    }
    // FASE 2: CROSSFADE ALUS! (5.5 detik)
    else if (elapsedTime > 5.5f && elapsedTime <= 11.0f) {
        float t = (elapsedTime - 5.5f) / 5.5f;

        // Perlahan memudarkan (fade out) bulan biasa
        DrawBulanBiasa(cx, cy, radius, rotasiGlobal, pulse, 1.0f - t); 
        
        // Secara paralel memunculkan bulan Mugen Tsukuyomi (fade in) di atasnya
        DrawBulanMerah(cx, cy, radius, rotasiGlobal, pulse, t);
    }
    // FASE 3: MUGEN TSUKUYOMI AKTIF PENUH
    else {
        DrawBulanMerah(cx, cy, radius, rotasiGlobal, pulse, 1.0f);
    }
}

// =========================================================
// UCHIHA MADARA RIKUDOU MODE
// =========================================================
void DrawMadaraRikudou(int cx, int cy, float scale, float elapsedTime) {
    float rotasiGudodama = elapsedTime * 0.8f;   
    for (int i = 0; i < 10; i++) {
        float angle = rotasiGudodama + (i * PI * 2.0f / 10.0f);
        int radiusMelingkar = (int)(65 * scale); 
        int gx = cx + (int)(cosf(angle) * radiusMelingkar);
        int gy = cy + (int)(sinf(angle) * radiusMelingkar * 0.7f); 
        
        DrawCircle(gx, gy, (int)(7 * scale), BLACK);
        DrawCircleLines(gx, gy, (int)(7 * scale), LIGHTGRAY); 
    }

    // Aura Glow Putih (Pulsating)
    float pulse = (sinf(elapsedTime * 4.0f) + 1.0f) * 0.5f; 
    int maxGlow = (int)(120 * scale);
    for (int r = (int)(40 * scale); r < maxGlow; r += 6) {
        int alpha = (int)(60 * pulse * (1.0f - (float)(r - 40*scale)/(maxGlow - 40*scale)));
        DrawCircle(cx, cy, r, (Color){255, 255, 255, (unsigned char)alpha});
    }

    // Tongkat Shakujo Hitam (Dipegang di sebelah kanan)
    DrawLineEx((Vector2){cx + 10*scale, cy + 60*scale}, (Vector2){cx + 70*scale, cy - 60*scale}, 5*scale, BLACK);
    DrawCircleLines(cx + 70*scale, cy - 60*scale, 12*scale, BLACK);
    DrawCircleLines(cx + 70*scale, cy - 60*scale, 13*scale, BLACK); 
    // Cincin kecil di tongkat
    DrawCircleLines(cx + 82*scale, cy - 60*scale, 4*scale, BLACK);
    DrawCircleLines(cx + 58*scale, cy - 60*scale, 4*scale, BLACK);

    // Kaki Hitam celana Uchiha
    Color pantsCol = (Color){20, 20, 25, 255}; 
    Bres_ThickLine(cx - 15*scale, cy + 60*scale, cx - 15*scale, cy + 120*scale, 16*scale, pantsCol); 
    Bres_ThickLine(cx + 15*scale, cy + 60*scale, cx + 15*scale, cy + 120*scale, 16*scale, pantsCol); 

    // Jubah Rikudou Senjutsu (Putih Terang)
    Color cloakCol = (Color){240, 240, 245, 255};
    // Badan/Kerah
    Vector2 v1 = { cx - 25*scale, cy - 10*scale }; 
    Vector2 v2 = { cx + 25*scale, cy - 10*scale }; 
    Vector2 v3 = { cx + 50*scale, cy + 70*scale }; 
    Vector2 v4 = { cx - 40*scale, cy + 80*scale }; 
    DrawTriangle(v1, v4, v3, cloakCol);
    DrawTriangle(v1, v3, v2, cloakCol);
    // Tangan
    DrawLineEx((Vector2){cx + 15*scale, cy}, (Vector2){cx + 40*scale, cy + 15*scale}, 18*scale, cloakCol);

    // Punggung Tomoe
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int px = cx - 12*scale + (j * 12 * scale);
            int py = cy + 10*scale + (i * 15 * scale);
            DrawCircle(px, py, 2.5f*scale, BLACK);
        }
    }

    Color hairCol = (Color){210, 210, 225, 255};
    DrawTriangle((Vector2){cx - 15*scale, cy - 30*scale}, (Vector2){cx - 45*scale, cy + 40*scale}, (Vector2){cx + 10*scale, cy - 10*scale}, hairCol);
    DrawTriangle((Vector2){cx + 10*scale, cy - 30*scale}, (Vector2){cx - 15*scale, cy - 10*scale}, (Vector2){cx + 35*scale, cy + 30*scale}, hairCol);
    DrawTriangle((Vector2){cx - 10*scale, cy - 40*scale}, (Vector2){cx - 55*scale, cy + 10*scale}, (Vector2){cx + 5*scale, cy - 10*scale}, hairCol);
    DrawCircle(cx - 5*scale, cy - 25*scale, 14*scale, (Color){180, 180, 190, 255});
    DrawTriangle((Vector2){cx - 12*scale, cy - 35*scale}, (Vector2){cx - 16*scale, cy - 48*scale}, (Vector2){cx - 8*scale, cy - 35*scale}, cloakCol);
}

// =========================================================
// Poligon
// =========================================================
static void DrawTriBoth(Vector2 a, Vector2 b, Vector2 c, Color color) {
    DrawTriangle(a, b, c, color);
    DrawTriangle(a, c, b, color);
}

static void FillQuad(int cx, int cy, float scale, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, Color c) {
    Vector2 p1 = {cx + x1*scale, cy + y1*scale};
    Vector2 p2 = {cx + x2*scale, cy + y2*scale};
    Vector2 p3 = {cx + x3*scale, cy + y3*scale};
    Vector2 p4 = {cx + x4*scale, cy + y4*scale};
    DrawTriBoth(p1, p2, p3, c);
    DrawTriBoth(p1, p3, p4, c);
}

static void FillTri(int cx, int cy, float scale, float x1, float y1, float x2, float y2, float x3, float y3, Color c) {
    Vector2 p1 = {cx + x1*scale, cy + y1*scale};
    Vector2 p2 = {cx + x2*scale, cy + y2*scale};
    Vector2 p3 = {cx + x3*scale, cy + y3*scale};
    DrawTriBoth(p1, p2, p3, c);
}

static void LineR(int cx, int cy, float scale, float x1, float y1, float x2, float y2, float thick, Color c) {
    DrawLineEx((Vector2){cx + x1*scale, cy + y1*scale}, (Vector2){cx + x2*scale, cy + y2*scale}, thick*scale, c);
}

// =========================================================
// NARUTO KURAMA MODE
// =========================================================
void DrawNarutoKurama(int cx, int cy, float scale, float elapsedTime) {
    // Double-sine wave interpolation untuk levitasi organik (dinaikkan amplitudonya agar semulus Madara)
    float wave1 = sinf(elapsedTime * 1.8f) * 15.0f;
    float wave2 = sinf(elapsedTime * 3.2f) * 5.0f;
    cy = cy + (int)(wave1 + wave2);

    Color bodyCol = GetColor(0xffeb99ff);
    Color outlineCol = GetColor(0xff6600ff);
    Color markCol = GetColor(0x111111ff);
    Color tailCol = GetColor(0xffeb99ff); 

    // Aura
    for (int r = (int)(90*scale); r > (int)(40*scale); r -= 10) {
        float alpha = (float)(r - 40*scale) / (50*scale);
        DrawCircle(cx, cy - 50*scale, r, Fade(outlineCol, (1.0f - alpha)*0.5f));
    }

    // Ekor
    for (int i = 0; i < 9; i++) {
        float angle = -PI / 2.0f + (i - 4) * 0.35f + sinf(elapsedTime * 3.0f + i) * 0.05f;
        // Garis Luar
        for (float t = 0; t <= 1.0f; t += 0.05f) {
            float py = -20.0f - (t * 120.0f);
            float rx = 0, ry = py; RotatePoint(&rx, &ry, 0, 0, angle);
            float radius = sinf(t * PI) * 22.0f * scale;
            if(radius > 1.0f) DrawCircle((int)(cx + rx*scale), (int)(cy + ry*scale), radius + 3*scale, outlineCol);
        }
        // Bagian Dalam
        for (float t = 0; t <= 1.0f; t += 0.05f) {
            float py = -20.0f - (t * 120.0f);
            float rx = 0, ry = py; RotatePoint(&rx, &ry, 0, 0, angle);
            float radius = sinf(t * PI) * 22.0f * scale;
            if(radius > 1.0f) DrawCircle((int)(cx + rx*scale), (int)(cy + ry*scale), radius, tailCol);
        }
        // Garis Titik Hitam
        for (float t = 0; t <= 1.0f; t += 0.08f) {
            float u = 1.0f - t;
            float px = u*u*0 + 2*u*t*5 + t*t*0; 
            float py = u*u*(-30) + 2*u*t*(-80) + t*t*(-130);
            float rx = px, ry = py; RotatePoint(&rx, &ry, 0, 0, angle);
            DrawCircle((int)(cx + rx*scale), (int)(cy + ry*scale), 2.0f * scale, markCol);
        }
    }

    // Kaki
    LineR(cx,cy,scale,-15,-10,-30,30,8,outlineCol);
    LineR(cx,cy,scale,-30,30,-10,30,8,outlineCol);
    LineR(cx,cy,scale,-10,30,-5,0,8,outlineCol);
    FillTri(cx,cy,scale,-15,-10,-30,30,-10,30,bodyCol);
    FillTri(cx,cy,scale,-15,-10,-10,30,-5,0,bodyCol);
    LineR(cx,cy,scale,15,-10,30,30,8,outlineCol);
    LineR(cx,cy,scale,30,30,10,30,8,outlineCol);
    LineR(cx,cy,scale,10,30,5,0,8,outlineCol);
    FillTri(cx,cy,scale,15,-10,30,30,10,30,bodyCol);
    FillTri(cx,cy,scale,15,-10,10,30,5,0,bodyCol);

    // Tangan
    LineR(cx,cy,scale,-25,-40,-50,-10,8,outlineCol);
    LineR(cx,cy,scale,-50,-10,-35,-5,8,outlineCol);
    LineR(cx,cy,scale,-35,-5,-15,-25,8,outlineCol);
    FillTri(cx,cy,scale,-25,-40,-50,-10,-35,-5,bodyCol);
    FillTri(cx,cy,scale,-25,-40,-35,-5,-15,-25,bodyCol);
    LineR(cx,cy,scale,25,-40,50,-10,8,outlineCol);
    LineR(cx,cy,scale,50,-10,35,-5,8,outlineCol);
    LineR(cx,cy,scale,35,-5,15,-25,8,outlineCol);
    FillTri(cx,cy,scale,25,-40,50,-10,35,-5,bodyCol);
    FillTri(cx,cy,scale,25,-40,35,-5,15,-25,bodyCol);

    // Badan
    FillQuad(cx,cy,scale,-20,-50,20,-50,15,0,-15,0,bodyCol);
    LineR(cx,cy,scale,-20,-50,20,-50,4,outlineCol);
    LineR(cx,cy,scale,20,-50,15,0,4,outlineCol);
    LineR(cx,cy,scale,15,0,-15,0,4,outlineCol);
    LineR(cx,cy,scale,-15,0,-20,-50,4,outlineCol);

    // Corak Sendi
    int jX[] = {-20, 20, -12, 12};
    int jY[] = {-40, -40, -10, -10};
    for(int i=0; i<4; i++) {
        DrawCircle(cx+jX[i]*scale, cy+jY[i]*scale, 6*scale, outlineCol);
        DrawCircle(cx+jX[i]*scale, cy+jY[i]*scale, 5*scale, bodyCol);
        DrawCircle(cx+jX[i]*scale, cy+jY[i]*scale, 2*scale, markCol);
    }

    // Perut
    DrawCircleLines(cx, cy - 25*scale, 10*scale, markCol);
    DrawCircle(cx, cy - 25*scale, 4*scale, markCol);
    LineR(cx,cy,scale,-16,-36,-6,-30,2,markCol);
    LineR(cx,cy,scale,16,-36,6,-30,2,markCol);
    LineR(cx,cy,scale,-10,-14,-4,-20,2,markCol);
    LineR(cx,cy,scale,10,-14,4,-20,2,markCol);

    // Kepala
    FillQuad(cx,cy,scale,-15,-45,-20,-60,-15,-80,0,-75,bodyCol);
    FillQuad(cx,cy,scale,0,-75,15,-80,20,-60,15,-45,bodyCol);
    FillTri(cx,cy,scale,-10,-60,0,-45,10,-60,bodyCol);

    // Telinga
    FillTri(cx,cy,scale,-12,-78,-30,-110,-2,-75,bodyCol);
    FillTri(cx,cy,scale,12,-78,30,-110,2,-75,bodyCol);
    FillTri(cx,cy,scale,-20,-92,-30,-110,-12,-92,markCol);
    FillTri(cx,cy,scale,20,-92,30,-110,12,-92,markCol);

    // Corak
    LineR(cx,cy,scale,-15,-65,-5,-55,2,markCol);
    LineR(cx,cy,scale,15,-65,5,-55,2,markCol);
    LineR(cx,cy,scale,-22,-65,-12,-62,2,markCol);
    LineR(cx,cy,scale,-24,-60,-14,-58,2,markCol);
    LineR(cx,cy,scale,22,-65,12,-62,2,markCol);
    LineR(cx,cy,scale,24,-60,14,-58,2,markCol);

    // Mata
    DrawEllipse(cx-8*scale, cy-68*scale, 4*scale, 2*scale, RED);
    DrawEllipse(cx+8*scale, cy-68*scale, 4*scale, 2*scale, RED);
}

// =========================================================
// SASUKE PERFECT SUSANOO
// =========================================================
void DrawSasukeSusanoo(int cx, int cy, float scale, float elapsedTime) {
    // Double-sine wave interpolation untuk levitasi organik (dinaikkan amplitudonya agar semulus Madara)
    float wave1 = sinf(elapsedTime * 1.6f + 1.0f) * 15.0f;
    float wave2 = sinf(elapsedTime * 2.9f + 0.5f) * 5.0f;
    cy = cy + (int)(wave1 + wave2);

    Color bPurp = GetColor(0x9b59b6ff);
    Color dPurp = GetColor(0x5b2c6fff);
    Color lPurp = GetColor(0xd2b4deff);
    Color oPurp = GetColor(0x4a235aff);

    // Aura
    for (int r = (int)(90*scale); r > (int)(40*scale); r -= 10) {
        float alpha = (float)(r - 40*scale) / (50*scale);
        DrawCircle(cx, cy - 40*scale, r, Fade(bPurp, (1.0f - alpha)*0.5f));
    }

    // Sayap
    float flap = sinf(elapsedTime * 2.0f) * 0.05f;
    for (int isLeft = 0; isLeft <= 1; isLeft++) {
        float dir = isLeft ? -1.0f : 1.0f;
        float angle = (0.1f + flap) * dir;
        
        Vector2 pts[5] = {{10,-30}, {150,-100}, {180,-20}, {140,60}, {30,20}};
        for(int j=0; j<5; j++) {
            float rx = pts[j].x, ry = pts[j].y;
            RotatePoint(&rx, &ry, 0, 0, angle);
            pts[j] = (Vector2){cx + rx * dir * scale, cy + ry * scale};
        }

        Color wingGrad1 = Fade(bPurp, 0.85f);
        Color wingGrad2 = Fade(bPurp, 0.70f);
        Color wingGrad3 = Fade(bPurp, 0.55f);

        // Lapisan 1
        DrawTriBoth(pts[0], pts[1], pts[2], wingGrad1);
        DrawTriBoth(pts[0], pts[2], pts[3], wingGrad1);
        DrawTriBoth(pts[0], pts[3], pts[4], wingGrad1);
        
        // Lapisan 2
        Vector2 inner1[5];
        for(int j=0; j<5; j++) inner1[j] = (Vector2){pts[0].x + (pts[j].x - pts[0].x)*0.8f, pts[0].y + (pts[j].y - pts[0].y)*0.8f};
        DrawTriBoth(inner1[0], inner1[1], inner1[2], wingGrad2);
        DrawTriBoth(inner1[0], inner1[2], inner1[3], wingGrad2);
        DrawTriBoth(inner1[0], inner1[3], inner1[4], wingGrad2);

        // Lapisan 3
        Vector2 inner2[5];
        for(int j=0; j<5; j++) inner2[j] = (Vector2){pts[0].x + (pts[j].x - pts[0].x)*0.5f, pts[0].y + (pts[j].y - pts[0].y)*0.5f};
        DrawTriBoth(inner2[0], inner2[1], inner2[2], wingGrad3);
        DrawTriBoth(inner2[0], inner2[2], inner2[3], wingGrad3);
        DrawTriBoth(inner2[0], inner2[3], inner2[4], wingGrad3);
        
        // Garis Kisi Sayap
        for (int i=1; i<=7; i++) {
            float t = (float)i / 8.0f;
            float destX, destY;
            if(t < 0.5f) { 
                float st = t * 2.0f;
                destX = 150 + st*(180 - 150);
                destY = -100 + st*(-20 - -100);
            } else { 
                float st = (t - 0.5f) * 2.0f;
                destX = 180 + st*(140 - 180);
                destY = -20 + st*(60 - -20);
            }
            float rx1 = 20, ry1 = -10;
            float rx2 = rx1 + (destX - rx1) * 0.94f;
            float ry2 = ry1 + (destY - ry1) * 0.94f;
            RotatePoint(&rx1, &ry1, 0, 0, angle); RotatePoint(&rx2, &ry2, 0, 0, angle);
            LineR(cx,cy,1.0f, rx1*dir*scale, ry1*scale, rx2*dir*scale, ry2*scale, 1.5f*scale, lPurp);
        }
        for (int i=1; i<=7; i++) {
            float t = (float)i / 8.0f;
            float topX = 10 + t*(150 - 10);
            float topY = -30 + t*(-100 - -30);
            float botX = 30 + t*(140 - 30);
            float botY = 20 + t*(60 - 20);
            
            float midX = (topX + botX) * 0.5f;
            float midY = (topY + botY) * 0.5f;
            float rx1 = midX + (topX - midX) * 0.94f;
            float ry1 = midY + (topY - midY) * 0.94f;
            float rx2 = midX + (botX - midX) * 0.94f;
            float ry2 = midY + (botY - midY) * 0.94f;
            
            RotatePoint(&rx1, &ry1, 0, 0, angle); RotatePoint(&rx2, &ry2, 0, 0, angle);
            LineR(cx,cy,1.0f, rx1*dir*scale, ry1*scale, rx2*dir*scale, ry2*scale, 1.5f*scale, lPurp);
        }
        for(int j=0; j<5; j++) DrawLineEx(pts[j], pts[(j+1)%5], 3*scale, oPurp);
    }

    // Kaki
    FillQuad(cx,cy,scale,-15,10,-20,50,-5,50,0,20,bPurp);
    FillQuad(cx,cy,scale,0,20,5,50,20,50,15,10,bPurp);

    // Rok
    FillQuad(cx,cy,scale,-25,-10,25,-10,35,25,-35,25,bPurp);
    for(int i=-25; i<=25; i+=10) LineR(cx,cy,scale,i,-10,i*1.3f,25,2,oPurp);
    DrawRectangle((int)(cx-22*scale), (int)(cy-15*scale), (int)(44*scale), (int)(10*scale), dPurp);

    // Badan
    FillQuad(cx,cy,scale,-25,-45,25,-45,20,-15,-20,-15,bPurp);
    LineR(cx,cy,scale,0,-45,0,-15,2,oPurp);
    LineR(cx,cy,scale,-15,-35,15,-35,2,oPurp);
    LineR(cx,cy,scale,-18,-25,18,-25,2,oPurp);

    // Bahu
    for(float dir=-1; dir<=1; dir+=2) {
        FillTri(cx,cy,scale,20*dir,-45,50*dir,-60,60*dir,-30,bPurp);
        FillTri(cx,cy,scale,20*dir,-45,60*dir,-30,30*dir,-15,bPurp);
        LineR(cx,cy,scale,25*dir,-35,55*dir,-45,2,oPurp);
        LineR(cx,cy,scale,28*dir,-25,58*dir,-35,2,oPurp);
    }

    // Tangan
    FillQuad(cx,cy,scale,-30,-25,-45,10,-35,15,-20,-15,bPurp);
    FillQuad(cx,cy,scale,30,-25,45,10,35,15,20,-15,bPurp);

    // Kepala & Helm
    FillQuad(cx,cy,scale,-15,-45,-18,-65,18,-65,15,-45,bPurp); 
    
    // Pelindung Samping Helm
    FillTri(cx,cy,scale,-18,-65,-25,-40,-15,-45,dPurp);
    FillTri(cx,cy,scale,18,-65,25,-40,15,-45,dPurp);
    
    // Pelindung Rahang/Hidung
    FillTri(cx,cy,scale,-10,-45,0,-30,10,-45,bPurp);
    LineR(cx,cy,scale,-10,-45,0,-30,2,oPurp);
    LineR(cx,cy,scale,10,-45,0,-30,2,oPurp);

    // Tanduk di Helm
    FillTri(cx,cy,scale,-5,-65,0,-90,5,-65, GetColor(0xffcc00ff)); // Tanduk Tengah
    FillTri(cx,cy,scale,-20,-65,-35,-85,-10,-65, lPurp); // Tanduk Kiri
    FillTri(cx,cy,scale,20,-65,35,-85,10,-65, lPurp); // Tanduk Kanan

    // Mata
    DrawEllipse(cx-8*scale, cy-55*scale, 5*scale, 2*scale, YELLOW);
    DrawEllipse(cx+8*scale, cy-55*scale, 5*scale, 2*scale, YELLOW);

    // Sword
    LineR(cx,cy,scale,35,10,120,80,5,lPurp);
    LineR(cx,cy,scale,35,10,120,80,2,WHITE);
    LineR(cx,cy,scale,25,0,45,-20,4,dPurp);
}
