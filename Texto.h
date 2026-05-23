#ifndef TEXTO_H_INCLUDED
#define TEXTO_H_INCLUDED

#include "alfabeto.h"

// Dibuja un string mayúsculo a partir de (col, fila)
// Ignora caracteres que no sean A-Z o espacio
void textoDibujar(const Alfabeto* alf, const char* texto, int col, int fila);

#endif // TEXTO_H_INCLUDED
