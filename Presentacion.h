#ifndef PRESENTACION_H_INCLUDED
#define PRESENTACION_H_INCLUDED

#include "Alfabeto.h"

#define USERNAME_MAX_LEN 12

int presentacionEjecutar(const Alfabeto* alf, int ancho, int alto);
int usernameEjecutar(const Alfabeto* alf, char* nombreOut, int ancho, int alto);

#endif // PRESENTACION_H_INCLUDED
