#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>

#define PIXELES_X_LADO 8
#define PX_PADDING 0
#define ANCHO_DIGITO 3
#define ALTO_DIGITO  5

// Cada dígito es una matriz de 5 filas x 3 columnas
// 1 = píxel encendido, 0 = transparente
extern const uint8_t digitos[10][5][3];
extern const uint8_t letras_score[5][5][3];
extern const uint8_t letras_next[4][5][3];

#define FONDO 0
#define CE 1
#define AM 2
#define VI 3
#define VE 4
#define RO 5
#define AZ 6
#define NA 7
#define N 8

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void dibujar_numero(int numero, uint16_t px, uint16_t py, uint8_t color);
#endif
