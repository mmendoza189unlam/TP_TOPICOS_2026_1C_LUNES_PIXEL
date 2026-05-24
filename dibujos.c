#include "dibujos.h"
#include <stdio.h>
#include <string.h>

// Inicialización de variables de control de escala y desplazamiento
extern int escala_dibujo;

// Matrices de Sprites Bitmap
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

const uint8_t letras_level[5][5][3] = {
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // L
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
    {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0}}, // V
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // L
};

const uint8_t letras_lines[5][5][3] = {
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // L
    {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // I
    {{1,0,1},{1,1,1},{1,1,1},{1,0,1},{1,0,1}}, // N
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
};

const uint8_t letras_statistics[10][5][3] = {
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
    {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}}, // A
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
    {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // I
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
    {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // I
    {{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // C
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
};

const uint8_t letras_mayus[26][5][3] = {
    {{0,1,0},{1,0,1},{1,1,1},{1,0,1},{1,0,1}}, // A
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,1,0}}, // B
    {{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // C
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,1,0}}, // D
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1}}, // E
    {{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,0,0}}, // F
    {{1,1,1},{1,0,0},{1,0,1},{1,0,1},{1,1,1}}, // G
    {{1,0,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}}, // H
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{1,1,1}}, // I
    {{0,0,1},{0,0,1},{0,0,1},{1,0,1},{1,1,1}}, // J
    {{1,0,1},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}, // K
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // L
    {{1,0,1},{1,1,1},{1,1,1},{1,0,1},{1,0,1}}, // M
    {{1,0,1},{1,1,1},{1,1,1},{1,1,1},{1,0,1}}, // N
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // O
    {{1,1,1},{1,0,1},{1,1,1},{1,0,0},{1,0,0}}, // P
    {{1,1,1},{1,0,1},{1,0,1},{1,1,1},{0,0,1}}, // Q
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}, // R
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
    {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
    {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // U
    {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0}}, // V
    {{1,0,1},{1,0,1},{1,1,1},{1,1,1},{1,0,1}}, // W
    {{1,0,1},{1,0,1},{0,1,0},{1,0,1},{1,0,1}}, // X
    {{1,0,1},{1,0,1},{0,1,0},{0,1,0},{0,1,0}}, // Y
    {{1,1,1},{0,0,1},{0,1,0},{1,0,0},{1,1,1}}  // Z
};

void dibujar_texto(const char* txt, uint16_t px, uint16_t py, uint8_t color){

    int offset = 0;

    for(int k=0; txt[k]!=0; k++){

        char c = txt[k];

        if(c==' '){
            offset += 4* escala_dibujo;
            continue;
        }

        if(c<'A' || c>'Z')
            continue;

        int letra = c - 'A';

        for(int y=0;y<5;y++)
            for(int x=0;x<3;x++)
                if (letras_mayus[letra][y][x])
                    for(int sy=0; sy<escala_dibujo; sy++)
                        for(int sx=0; sx<escala_dibujo; sx++)
                            gbt_dibujar_pixel(px + offset + x * escala_dibujo + sx, py + y * escala_dibujo + sy, color);

        offset += 4* escala_dibujo;// Espaciado entre letras
    }
}

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    // Calcula la posición en base al offset del tablero y tamaño del bloque con escala
    int baseX = offsetX + oX * (PIXELES_X_LADO * escala_dibujo + PX_PADDING);
    int baseY = offsetY + oY * (PIXELES_X_LADO * escala_dibujo + PX_PADDING);

    for (int y = 0; y < PIXELES_X_LADO; y++) {
        for (int x = 0; x < PIXELES_X_LADO; x++) {
            uint8_t color = sprite[y][x];

            if (color != N) {
                // Multiplica los píxeles si escala_dibujo es mayor a 1
                for(int sy = 0; sy < escala_dibujo; sy++)
                    for(int sx = 0; sx < escala_dibujo; sx++)
                        gbt_dibujar_pixel(
                            baseX + x * escala_dibujo + sx,
                            baseY + y * escala_dibujo + sy,
                            color
                        );
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
                    for(int sy=0; sy<escala_dibujo; sy++)
                        for(int sx=0; sx<escala_dibujo; sx++)
                            gbt_dibujar_pixel(px + k * (ANCHO_DIGITO + 1) * escala_dibujo + x * escala_dibujo + sx, py + y * escala_dibujo + sy, color);
    }
}

void dibujar_numero_ceros7(int numero, uint16_t px, uint16_t py, uint8_t color) {
    char buf[10];
    sprintf(buf, "%07d", numero); // Formato clásico de marcador arcade (ej: 0000120)
    int len = strlen(buf);

    for (int k = 0; k < len; k++) {
        int d = buf[k] - '0';
        for (int y = 0; y < ALTO_DIGITO; y++) {
            for (int x = 0; x < ANCHO_DIGITO; x++) {
                if (digitos[d][y][x]) {
                    for(int sy=0; sy<escala_dibujo; sy++)
                        for(int sx=0; sx<escala_dibujo; sx++)
                            gbt_dibujar_pixel(px + k * (ANCHO_DIGITO + 1) * escala_dibujo + x * escala_dibujo + sx, py + y * escala_dibujo + sy, color);
                }
            }
        }
    }
}

void dibujar_numero_ceros3(int numero, uint16_t px, uint16_t py, uint8_t color) {
    char buf[10];
    sprintf(buf, "%03d", numero); // Formato clásico de marcador arcade (ej: 0000120)
    int len = strlen(buf);

    for (int k = 0; k < len; k++) {
        int d = buf[k] - '0';
        for (int y = 0; y < ALTO_DIGITO; y++) {
            for (int x = 0; x < ANCHO_DIGITO; x++) {
                if (digitos[d][y][x]) {
                    for(int sy=0; sy<escala_dibujo; sy++)
                        for(int sx=0; sx<escala_dibujo; sx++)
                            gbt_dibujar_pixel(px + k * (ANCHO_DIGITO + 1) * escala_dibujo + x * escala_dibujo + sx, py + y * escala_dibujo + sy, color);
                }
            }
        }
    }
}
