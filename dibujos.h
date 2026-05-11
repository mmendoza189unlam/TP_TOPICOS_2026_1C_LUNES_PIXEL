#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>

#define PIXELES_X_LADO 8
#define PX_PADDING 0
#define ANCHO_DIGITO 3
#define ALTO_DIGITO  5

// Cada d�gito es una matriz de 5 filas x 3 columnas
// 1 = p�xel encendido, 0 = transparente
extern const uint8_t digitos[10][5][3];
extern const uint8_t letras_score[5][5][3];
extern const uint8_t letras_next[4][5][3];
extern const uint8_t letras_level[5][5][3]; //Agregue este para el Level

#define N 15 // Transparente
#define A 14 // Amarillo
#define C 3  // Celeste
#define V 2  // Verde

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void dibujar_numero(int numero, uint16_t px, uint16_t py, uint8_t color);
void dibujar_numero_ceros(int numero, uint16_t px, uint16_t py, uint8_t color);
#endif

