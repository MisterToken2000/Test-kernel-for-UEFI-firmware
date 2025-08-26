#include <stdint.h>
#include <stdbool.h>

#define UINT16 uint16_t

void PutPixel(uint32_t x, uint32_t y, uint32_t color, uint32_t *FrameBuffer);

void ClearScreen(uint32_t color, uint32_t *FrameBuffer);

void Draw_Z(uint32_t *FrameBuffer, uint32_t Pitch, uint32_t x, uint32_t y, uint32_t Color);

void DrawFilledCircle(int cx, int cy, int radius, uint32_t color, uint32_t* frameBuffer, int width, int height);

void Draw_Unicode(uint32_t *FrameBuffer, char Char, uint32_t x, uint32_t y, uint32_t Color);

void main(uint32_t *FrameBuffer) {

        uint64_t Pixels = 1920 * 1080;
        for (uint64_t i = 0; i < Pixels; ++i) {
                FrameBuffer[i] = 0x00000000;
        }

        Draw_Unicode(FrameBuffer, 'A', 500, 500, 0xffffffff);
        while (true);
}

double isqrt(double n) {
    double x = n;
    double y = 1;
    double e = 0.000001; // точность

    while (x - y > e) {
        x = (x + y) / 2;
        y = n / x;
    }

    return x;
}

void Draw_Z(uint32_t *FrameBuffer, uint32_t Pitch, uint32_t x, uint32_t y, uint32_t Color) {
         uint16_t z_glyph[32] = {
                0xaa, 0xaa, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x4a, 0x51, 0xea, 0x50, 0x5a, 0x51,
                0xc9, 0x9e, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x55, 0x55
         };

        for (uint64_t Row = 0; Row < 32; Row++) {
                for (uint64_t Col = 0; Col < 16; Col++) {
                        if (z_glyph[Row] & (1 << (15 - Col))) {
                                FrameBuffer[(y + Row) * Pitch + (x + Col)] = Color;
                        }
                }
        }
}

void DrawFilledCircle(int cx, int cy, int radius, uint32_t color, uint32_t* FrameBuffer, int width, int height) {
    for (int y = -radius; y <= radius; y++) {
        int dy = cy + y;
        if (dy < 0 || dy >= height) continue;

        // Вычисляем ширину сканлайна
        int dx_limit = (int)isqrt(radius * radius - y * y);

        for (int x = -dx_limit; x <= dx_limit; x++) {
            int dx = cx + x;
            if (dx >= 0 && dx < width) {
                PutPixel(dx, dy, color, FrameBuffer);
            }
        }
    }
}

void PutPixel(uint32_t x, uint32_t y, uint32_t color, uint32_t *FrameBuffer) {
        FrameBuffer[y * 1920 + x] = color;
}

void ClearScreen(uint32_t color, uint32_t *FrameBuffer) {
        uint64_t Pixels = 1920 * 1080;
        for (uint64_t i = 0; i < Pixels; ++i) {
                FrameBuffer[i] = color;
        }
}

void Draw_Unicode(uint32_t *FrameBuffer, char Char, uint32_t x, uint32_t y, uint32_t Color) {
    uint32_t Pitch = 1920;
    uint16_t *char_bitmap;

    /*
    if (Char == 'A') char_bitmap = a_upper;
    else if (Char == 'a') char_bitmap = a_lower;
    */

    for (uint64_t Row = 0; Row < 16; Row++) {
        for (uint64_t Col = 0; Col < 16; Col++) {
            if (char_bitmap[Row] & (2 << (14 - Col))) {
                uint32_t *Pixel = FrameBuffer;
                Pixel[(y + Row) * Pitch + (x + Col)] = Color;
            }
        }
   }
}
