#include "Texto.h"

void textoDibujar(const Alfabeto* alf, const char* texto, int col, int fila)
{
    int c = col;
    int i;
    for(i = 0; texto[i] != '\0'; i++)
    {
        const Letra* l = NULL;

        if(texto[i] >= 'A' && texto[i] <= 'Z')
        {
            l = alfabetoObtenerLetra(alf, texto[i]);
        }
        else
        {
            if(texto[i] >= '0' && texto[i] <= '9')
            {
                l = alfabetoObtenerNumero(alf, texto[i] - '0');
            }
        }

        if(l != NULL)
        {
            letraDibujar(l, c, fila);
        }
        c++;    // avanza siempre, espacio incluido
    }
}
