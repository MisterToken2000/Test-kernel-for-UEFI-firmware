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

        // PutPixel(100, 100, 0xffffffff, FrameBuffer);
        Draw_Z(FrameBuffer, 1920, 100, 100, 0xffffffff);
	// DrawFilledCircle(960, 540, 50, 0xffffffff, FrameBuffer, 1920, 1080);
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

    UINT16 a_upper[16] = {
        0xaa, 0xaa, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x4a, 0x51, 0xea, 0x50, 0x5a, 0x51
    };

UINT16 a_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b10000010,
        0b00000010,
        0b01111110,
        0b10000010,
        0b10000010,
        0b10000110,
        0b01111010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 b_upper[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111000,
        0b10000100,
        0b10000100,
        0b10000100,
        0b11111000,
        0b10000100,
        0b10000100,
        0b10000100,
        0b10000100,
        0b11111000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 b_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10111100,
        0b11000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b11000010,
        0b10111100,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 c_upper[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01000010,
        0b10000010,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000010,
        0b01000010,
        0b00111100,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 c_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111100,
        0b01000010,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b01000010,
        0b00111100,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 d_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111000,
        0b10000100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000100,
        0b11111000,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 d_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00111110,
        0b01000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000110,
        0b00111010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 e_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111110,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11111110,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11111110,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 e_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b10000010,
        0b10000010,
        0b11111110,
        0b10000000,
        0b10000000,
        0b10000010,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 f_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111110,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11111100,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 f_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00011110,
        0b00100000,
        0b00100000,
        0b00100000,
        0b11111110,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 g_upper[16] = {
        0b00000000,
        0b00000000,
        0b01111100,
        0b10000010,
        0b10000010,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10011110,
        0b10000010,
        0b10000010,
        0b10000110,
        0b01111010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 g_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000010,
        0b01111010,
        0b10000100,
        0b10000100,
        0b10000100,
        0b01111000,
        0b01000000,
        0b01111100,
        0b10000010,
        0b10000010,
        0b01111100
};

UINT16 h_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b11111110,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 h_lower[16] = {
        0b00000000,
        0b00000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10111100,
        0b11000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 i_upper[16] = {
        0b00000000,
        0b00000000,
        0b01111100,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 i_lower[16] = {
        0b00000000,
        0b00000000,
        0b00010000,
        0b00010000,
        0b00000000,
        0b00110000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 j_upper[16] = {
        0b00000000,
        0b00000000,
        0b00111110,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b10001000,
        0b01110000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 j_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000010,
        0b00000010,
        0b00000000,
        0b00000110,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b10000100,
        0b01111000,
        0b00000000
};

UINT16 k_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000100,
        0b10001000,
        0b10010000,
        0b10100000,
        0b11000000,
        0b10100000,
        0b10010000,
        0b10001000,
        0b10000100,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 k_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000100,
        0b10001000,
        0b10010000,
        0b11100000,
        0b10010000,
        0b10001000,
        0b10000100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 l_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11111111,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 l_lower[16] = {
        0b00000000,
        0b00000000,
        0b00110000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 m_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b11000110,
        0b10101010,
        0b10010010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 m_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10101100,
        0b11010010,
        0b10010010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 n_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b11000010,
        0b11000010,
        0b10100010,
        0b10100010,
        0b10010010,
        0b10010010,
        0b10001010,
        0b10001010,
        0b10000110,
        0b10000110,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 n_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10011100,
        0b10100010,
        0b11000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 o_upper[16] = {
        0b00000000,
        0b00000000,
        0b00111000,
        0b01000100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000100,
        0b00111000,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 o_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111000,
        0b01000100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000100,
        0b00111000,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 p_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b11111100,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 p_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10111100,
        0b11000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b11000010,
        0b10111100,
        0b10000000,
        0b10000000,
        0b10000000,
        0b00000000
};

UINT16 q_upper[16] = {
        0b00000000,
        0b00000000,
        0b00111000,
        0b01000100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10010010,
        0b10001010,
        0b01000100,
        0b00111010,
        0b00000000,
        0b00000000,
        0b00000000,
};

UINT16 q_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00111010,
        0b01000110,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000110,
        0b00111010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b00000000
};

UINT16 r_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111100,
        0b10000010,
        0b10000010,
        0b10000010,
        0b11111100,
        0b10001000,
        0b10001000,
        0b10000100,
        0b10000100,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 r_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10011100,
        0b10100010,
        0b11000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 s_upper[16] = {
        0b00000000,
        0b00000000,
        0b01111100,
        0b10000010,
        0b10000010,
        0b10000000,
        0b01000000,
        0b00110000,
        0b00001100,
        0b00000010,
        0b10000010,
        0b10000010,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 s_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b01111100,
        0b10000010,
        0b10000000,
        0b01110000,
        0b00001100,
        0b00000010,
        0b10000010,
        0b01111100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 t_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111110,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 t_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00010000,
        0b00010000,
        0b01111100,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00001100,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 u_upper[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000100,
        0b00111000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 u_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000110,
        0b00111010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 v_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b01000100,
        0b01000100,
        0b01000100,
        0b01001100,
        0b00101000,
        0b00101000,
        0b00101000,
        0b00010000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 v_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b01000100,
        0b01000100,
        0b01000100,
        0b00101000,
        0b00101000,
        0b00010000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 w_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10010010,
        0b10101010,
        0b11000110,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 w_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10010010,
        0b10101010,
        0b11000110,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 x_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b01000100,
        0b01000100,
        0b00101000,
        0b00010000,
        0b00101000,
        0b01000100,
        0b01000100,
        0b10000010,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 x_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b01000100,
        0b00101000,
        0b00010000,
        0b00101000,
        0b01000100,
        0b10000010,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 y_upper[16] = {
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b01000100,
        0b01000100,
        0b00101000,
        0b00101000,
        0b00101000,
        0b00010000,
        0b00010000,
        0b001000000,
        0b01100000,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 y_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000010,
        0b10000110,
        0b01111010,
        0b00000010,
        0b00000010,
        0b00000010,
        0b01111100
};

UINT16 z_upper[16] = {
        0b00000000,
        0b00000000,
        0b11111110,
        0b00000110,
        0b00001000,
        0b00010000,
        0b00010000,
        0b00100000,
        0b00100000,
        0b01000000,
        0b01000000,
        0b10000000,
        0b11111110,
        0b00000000,
        0b00000000,
        0b00000000
};

UINT16 z_lower[16] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b11111110,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b11111110,
        0b00000000,
        0b00000000,
        0b00000000
};

    if (Char == 'A') char_bitmap = a_upper;
    else if (Char == 'a') char_bitmap = a_lower;
    else if (Char == 'B') char_bitmap = b_upper;
    else if (Char == 'b') char_bitmap = b_lower;
    else if (Char == 'C') char_bitmap = c_upper;
    else if (Char == 'c') char_bitmap = c_lower;
    else if (Char == 'D') char_bitmap = d_upper;
    else if (Char == 'd') char_bitmap = d_lower;
    else if (Char == 'E') char_bitmap = e_upper;
    else if (Char == 'e') char_bitmap = e_lower;
    else if (Char == 'F') char_bitmap = f_upper;
    else if (Char == 'f') char_bitmap = f_lower;
    else if (Char == 'G') char_bitmap = g_upper;
    else if (Char == 'g') char_bitmap = g_lower;
    else if (Char == 'H') char_bitmap = h_upper;
    else if (Char == 'h') char_bitmap = h_lower;
    else if (Char == 'I') char_bitmap = i_upper;
    else if (Char == 'i') char_bitmap = i_lower;
    else if (Char == 'J') char_bitmap = j_upper;
    else if (Char == 'j') char_bitmap = j_lower;
    else if (Char == 'K') char_bitmap = k_upper;
    else if (Char == 'k') char_bitmap = k_lower;
    else if (Char == 'L') char_bitmap = l_upper;
    else if (Char == 'l') char_bitmap = l_lower;
    else if (Char == 'M') char_bitmap = m_upper;
    else if (Char == 'm') char_bitmap = m_lower;
    else if (Char == 'N') char_bitmap = n_upper;
    else if (Char == 'n') char_bitmap = n_lower;
    else if (Char == 'O') char_bitmap = o_upper;
    else if (Char == 'o') char_bitmap = o_lower;
    else if (Char == 'P') char_bitmap = p_upper;
    else if (Char == 'p') char_bitmap = p_lower;
    else if (Char == 'Q') char_bitmap = q_upper;
    else if (Char == 'q') char_bitmap = q_lower;
    else if (Char == 'R') char_bitmap = r_upper;
    else if (Char == 'r') char_bitmap = r_lower;
    else if (Char == 'S') char_bitmap = s_upper;
    else if (Char == 's') char_bitmap = s_lower;
    else if (Char == 'T') char_bitmap = t_upper;
    else if (Char == 't') char_bitmap = t_lower;
    else if (Char == 'U') char_bitmap = u_upper;
    else if (Char == 'u') char_bitmap = u_lower;
    else if (Char == 'V') char_bitmap = v_upper;
    else if (Char == 'v') char_bitmap = v_lower;
    else if (Char == 'W') char_bitmap = w_upper;
    else if (Char == 'w') char_bitmap = w_lower;
    else if (Char == 'X') char_bitmap = x_upper;
    else if (Char == 'x') char_bitmap = x_lower;
    else if (Char == 'Y') char_bitmap = y_upper;
    else if (Char == 'y') char_bitmap = y_lower;
    else if (Char == 'Z') char_bitmap = z_upper;
    else if (Char == 'z') char_bitmap = z_lower;
    /* else if (Char == '0') char_bitmap = null_number;
    else if (Char == '1') char_bitmap = one_number;
    else if (Char == '2') char_bitmap = two_number;
    else if (Char == '3') char_bitmap = three_number;
    else if (Char == '4') char_bitmap = four_number;
    else if (Char == '5') char_bitmap = five_number;
    else if (Char == '6') char_bitmap = six_number;
    else if (Char == '7') char_bitmap = seven_number;
    else if (Char == '8') char_bitmap = eight_number;
    else if (Char == '9') char_bitmap = nine_number;
    else if (Char == ',') char_bitmap = comma_symbol;
    else if (Char == '.') char_bitmap = dot_symbol;
    else if (Char == '?') char_bitmap = question_mark_symbol;
    else if (Char == '!') char_bitmap = exclamation_mark_symbol;
    else if (Char == ':') char_bitmap = colon_symbol;
    else if (Char == '-') char_bitmap = hyphen_symbol;
    else char_bitmap = unknown_symbol;
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
