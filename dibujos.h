#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>

#define PIXELES_X_LADO 8
#define PX_PADDING 0

#define N 15 // Transparente
#define A 14 // Amarillo
#define C 3  // Celeste
#define V 2  // Verde

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);

#endif
