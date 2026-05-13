#include "dibujos.h"
#include <stdio.h>
#include <string.h>

const uint8_t digitos[10][5][3] = {
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // 0
    {{0,1,0},{1,1,0},{0,1,0},{0,1,0},{1,1,1}}, // 1
    {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}}, // 2
    {{1,1,1},{0,0,1},{0,1,1},{0,0,1},{1,1,1}}, // 3
    {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}}, // 4
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // 5
    {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}}, // 6
    {{1,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}}, // 7
    {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}}, // 8
    {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}}, // 9
};

const uint8_t letras_score[5][5][3] = {
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
    {{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // C
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // O
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}, // R
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
};

const uint8_t letras_next[4][5][3] = {
    {{1,0,1},{1,1,1},{1,1,1},{1,0,1},{1,0,1}}, // N
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
    {{1,0,1},{1,0,1},{0,1,0},{1,0,1},{1,0,1}}, // X
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
};

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    int baseX = oX * (PIXELES_X_LADO + PX_PADDING);
    int baseY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (int y = 0; y < PIXELES_X_LADO; y++) {
        for (int x = 0; x < PIXELES_X_LADO; x++) {

            uint8_t color = sprite[y][x];

            if (color != N) {
                gbt_dibujar_pixel(baseX + x, baseY + y, color);
            }
        }
    }
}


void dibujar_numero(int numero, uint16_t px, uint16_t py, uint8_t color) {
    char buf[16];
    sprintf(buf, "%d", numero);
    int len = strlen(buf);
    for (int k = 0; k < len; k++) {
        int d = buf[k] - '0';
        for (int y = 0; y < ALTO_DIGITO; y++)
            for (int x = 0; x < ANCHO_DIGITO; x++)
                if (digitos[d][y][x])
                    gbt_dibujar_pixel(px + k*(ANCHO_DIGITO+1) + x, py + y, color);
    }
}
