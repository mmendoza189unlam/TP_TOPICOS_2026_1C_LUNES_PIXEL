#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <stdint.h>
#include "Alfabeto.h"

// ── Dimensiones del tablero ───────────────────────────────────
#define ANCHO_TABLERO  10
#define ALTO_VISIBLE   20
#define FILAS_OCULTAS   4
#define ALTO_TABLERO   (ALTO_VISIBLE + FILAS_OCULTAS)
#define CANT_PIEZAS     7

// Retorna 1 si el jugador salió, 0 si fue game over
int tetrisEjecutar(const Alfabeto* alf, const char* nombreJugador, int ancho, int alto);

#endif // TETRIS_H_INCLUDED
