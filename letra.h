#ifndef LETRA_H_INCLUDED
#define LETRA_H_INCLUDED

#include <stdint.h>
#include "dibujos.h"

typedef struct
{
    uint8_t pixeles[8][PIXELES_X_LADO];
}
Letra;

void letraCrear(Letra* l, const uint8_t fuente[][PIXELES_X_LADO]);
void letraDibujar(const Letra* l, int col, int fila);

#endif // LETRA_H_INCLUDED
