#include "Alfabeto.h"
#define A 2
#define TR 0  // Índice de nuestro color Amarillo
/* Se plasman las letras de la a-z en matrices de 8x8 */
static const uint8_t fuenteLetras[CANT_LETRAS][8][PIXELES_X_LADO] =
{
  {   /* Letra A */
        {TR, TR, A, A, A, A, TR, TR},
        {TR, A, A, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra B */
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, A, A, A, A, TR, TR}
    },
    {   /* Letra C */
        {TR, A, A, A, A, A, A, TR},
        {A, A, TR, TR, TR, TR, TR, A},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* Letra D */
        {A, A, A, A, A, TR, TR, TR},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, A, A, A, TR, TR, TR}
    },
    {   /* Letra E */
        {A, A, A, A, A, A, A, A},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra F */
        {A, A, A, A, A, A, A, A},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR}
    },
    {   /* Letra G */
        {TR, A, A, A, A, A, A, TR},
        {A, A, TR, TR, TR, TR, TR, A},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, A, A, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* Letra H */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A}
    },

    {   /* Letra I */
        {A, A, A, A, A, A, A, A},
        {TR, TR, A, A, A, A, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, A, A, A, A, TR, TR},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra J */
        {TR, TR, TR, A, A, A, A, A},
        {TR, TR, TR, TR, TR, A, A, A},
        {TR, TR, TR, TR, TR, A, A, A},
        {TR, TR, TR, TR, TR, A, A, A},
        {A, A, TR, TR, TR, A, A, A},
        {A, A, TR, TR, TR, A, A, A},
        {A, A, TR, TR, TR, A, A, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* Letra K */
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, A, A, TR, TR},
        {A, A, TR, A, A, TR, TR, TR},
        {A, A, A, A, TR, TR, TR, TR},
        {A, A, A, A, TR, TR, TR, TR},
        {A, A, TR, A, A, TR, TR, TR},
        {A, A, TR, TR, A, A, TR, TR},
        {A, A, TR, TR, TR, A, A, TR}
    },
    {   /* Letra L */
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra M */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, TR, TR, A, A, A},
        {A, A, TR, A, A, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra TR */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, TR, TR, TR, A, A},
        {A, A, TR, A, TR, TR, A, A},
        {A, A, TR, TR, A, TR, A, A},
        {A, A, TR, TR, TR, A, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra O */
        {TR, A, A, A, A, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* Letra P */
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR}
    },
    {   /* Letra Q */
        {TR, A, A, A, A, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, A, TR, A, A},
        {A, A, TR, TR, TR, A, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, A, A, A, A, A}
    },
    {   /* Letra R */
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, TR, TR},
        {A, A, TR, A, A, TR, TR, TR},
        {A, A, TR, TR, A, A, TR, TR},
        {A, A, TR, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra S */
        {TR, A, A, A, A, A, A, A},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {TR, A, A, A, A, A, A, TR},
        {TR, TR, TR, TR, TR, TR, A, A},
        {TR, TR, TR, TR, TR, TR, A, A},
        {TR, TR, TR, TR, TR, TR, A, A},
        {A, A, A, A, A, A, A, TR}
    },
    {   /* Letra T */
        {A, A, A, A, A, A, A, A},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR}
    },
    {   /* Letra U */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* Letra V */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, TR, TR, A, A, TR},
        {TR, A, A, TR, TR, A, A, TR},
        {TR, TR, A, A, A, A, TR, TR},
        {TR, TR, A, A, A, A, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR}
    },
    {   /* Letra W */
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, TR, TR, TR, A, A},
        {A, A, TR, A, A, TR, A, A},
        {A, A, TR, A, A, TR, A, A},
        {A, A, A, TR, TR, A, A, A},
        {A, A, A, TR, TR, A, A, A},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra X */
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, TR, TR, A, A, TR},
        {TR, TR, A, A,A, A, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, A, A, A, A, TR, TR},
        {TR, A, A, TR, TR, A, A, TR},
        {A, A, TR, TR, TR, TR, A, A}
    },
    {   /* Letra Y */
        {A, A, TR, TR, TR, TR, A, A},
        {TR, A, A, TR, TR, A, A, TR},
        {TR, TR, A, A, A, A, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR}
    },
    {   /* Letra Z */
        {A, A, A, A, A, A, A, A},
        {TR, TR, TR, TR, TR, A, A, TR},
        {TR, TR, TR, TR, A, A, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, A, A, TR, TR, TR, TR},
        {TR, A, A, TR, TR, TR, TR, TR},
        {A, A, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, A}
    }
};

/* Se plasman los numeros del 0 al 9 en matrices de 8x8 */
static const uint8_t fuenteNumeros[CANT_NUMEROS][8][PIXELES_X_LADO] =
{
    {   /* TRumero 0 */
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* TRumero 1 */
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, A, A, A, TR, TR, TR},
        {TR, A, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {TR, TR, TR, A, A, TR, TR, TR},
        {A, A, A, A, A, A, A, A}
    },
    {   /* TRumero 2 */
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, A, TR},
        {TR, TR, TR, TR, TR, A, TR, TR},
        {TR, TR, TR, TR, A, TR, TR, TR},
        {TR, TR, TR, A, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, A}
    },
    {   /* TRumero 3 */
        {A, A, A, A, A, A, A, TR},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, A, A, A, TR},
        {TR, TR, TR, A, A, A, A, TR},
        {TR, TR, TR, TR, A, A, A, TR},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* TRumero 4 */
        {TR, TR, TR, TR, A, A, TR, TR},
        {TR, TR, TR, A, A, A, TR, TR},
        {TR, TR, A, TR, A, A, TR, TR},
        {TR, A, TR, TR, A, A, TR, TR},
        {A, A, A, A, A, A, A, A},
        {TR, TR, TR, TR, A, A, TR, TR},
        {TR, TR, TR, TR, A, A, TR, TR},
        {TR, TR, TR, TR, A, A, TR, TR}
    },
    {   /* TRumero 5 */
        {A, A, A, A, A, A, A, A},
        {A, TR, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, TR},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* TRumero 6 */
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, TR},
        {A, TR, TR, TR, TR, TR, TR, TR},
        {A, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* TRumero 7 */
        {A, A, A, A, A, A, A, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, A, TR},
        {TR, TR, TR, TR, TR, A, TR, TR},
        {TR, TR, TR, TR, A, TR, TR, TR},
        {TR, TR, TR, A, TR, TR, TR, TR},
        {TR, TR, A, TR, TR, TR, TR, TR},
        {TR, A, TR, TR, TR, TR, TR, TR}
    },
    {   /* TRumero 8 */
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    },
    {   /* TRumero 9 */
        {TR, A, A, A, A, A, A, TR},
        {A, TR, TR, TR, TR, TR, TR, A},
        {A, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, TR, TR, TR, TR, TR, TR, A},
        {TR, A, A, A, A, A, A, TR}
    }
};

void alfabetoCrear(Alfabeto* alf)
{
    int i;
    for(i = 0; i < CANT_LETRAS; i++)
    {
        letraCrear(&alf->letras[i], fuenteLetras[i]);
    }

    for(i = 0; i < CANT_NUMEROS; i++)
    {
        letraCrear(&alf->numeros[i], fuenteNumeros[i]);
    }
}

const Letra* alfabetoObtenerLetra(const Alfabeto* alf, char c)
{
    if(c < 'A' || c > 'Z')
    {
        return NULL;
    }
    return &alf->letras[c - 'A'];
}

const Letra* alfabetoObtenerNumero(const Alfabeto* alf, int n)
{
    if(n < 0 || n > 9)
    {
        return NULL;
    }
    return &alf->numeros[n];
}
