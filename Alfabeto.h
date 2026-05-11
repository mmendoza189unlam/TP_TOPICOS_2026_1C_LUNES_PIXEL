#ifndef ALFABETO_H_INCLUDED
#define ALFABETO_H_INCLUDED

#include "Letra.h"

#define CANT_LETRAS  26
#define CANT_NUMEROS 10

typedef struct
{
    Letra letras[CANT_LETRAS];
    Letra numeros[CANT_NUMEROS];
}
Alfabeto;

void alfabetoCrear(Alfabeto* alf);
const Letra* alfabetoObtenerLetra(const Alfabeto* alf, char c);   // 'A'-'Z'
const Letra* alfabetoObtenerNumero(const Alfabeto* alf, int n);   //  0 - 9

#endif // ALFABETO_H_INCLUDED
