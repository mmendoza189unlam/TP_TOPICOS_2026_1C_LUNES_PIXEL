/*
 Texto.c - renderizado de cadenas de texto.
 Responsable de iterar sobre cadenas de caracteres y delegar el dibujo
 de cada carácter (letras o números) al módulo de Alfabeto.
 */

#include "Texto.h"

// Dibuja una cadena de texto en pantalla carácter por carácter.
void textoDibujar(const Alfabeto* alf, const char* texto, int col, int fila) {
    int c = col;
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        const Letra* l = NULL;

        // Selección de la fuente adecuada según el carácter (A-Z o 0-9)
        if (texto[i] >= 'A' && texto[i] <= 'Z') {
            l = alfabetoObtenerLetra(alf, texto[i]);
        } else {
            if (texto[i] >= '0' && texto[i] <= '9') {
                l = alfabetoObtenerNumero(alf, texto[i] - '0');
            }
        }

        // Si se obtuvo una representación gráfica válida, se procede a dibujar
        if (l != NULL) {
            letraDibujar(l, c, fila);
        }

        // Incremento de columna para el siguiente carácter
        c++;
    }
}
