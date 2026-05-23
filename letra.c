#include "Letra.h"
#include "GBT/gbt.h"
#include <string.h>

void letraCrear(Letra* l, const uint8_t fuente[][PIXELES_X_LADO])
{
    for(int f = 0; f < 8; f++)
        for(int c = 0; c < PIXELES_X_LADO; c++)
            l->pixeles[f][c] = fuente[f][c];
}

void letraDibujar(const Letra* l, int col, int fila)
{
    // Reutiliza tu función dibujar() existente — sin duplicar lógica
    dibujar(l->pixeles, col, fila);
}
