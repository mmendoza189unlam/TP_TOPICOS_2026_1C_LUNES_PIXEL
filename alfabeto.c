#include "Alfabeto.h"
#define A 2  // Índice de nuestro color Amarillo
#define N 0  // Índice de nuestro color Transparente (Negro)
/* Se plasman las letras de la a-z en matrices de 8x8 */
static const uint8_t fuenteLetras[CANT_LETRAS][8][PIXELES_X_LADO] =
{
  {   /* Letra A */
        {N, N, A, A, A, A, N, N},
        {N, A, A, N, N, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, A, A, A, A, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra B */
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, A, A, N},
        {A, A, A, A, A, A, N, N}
    },
    {   /* Letra C */
        {N, A, A, A, A, A, A, N},
        {A, A, N, N, N, N, N, A},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Letra D */
        {A, A, A, A, A, N, N, N},
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, A, A, N},
        {A, A, A, A, A, N, N, N}
    },
    {   /* Letra E */
        {A, A, A, A, A, A, A, A},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra F */
        {A, A, A, A, A, A, A, A},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N}
    },
    {   /* Letra G */
        {N, A, A, A, A, A, A, N},
        {A, A, N, N, N, N, N, A},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, A, A, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Letra H */
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, A, A, A, A, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A}
    },

    {   /* Letra I */
        {A, A, A, A, A, A, A, A},
        {N, N, A, A, A, A, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, A, A, A, A, N, N},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra J */
        {N, N, N, A, A, A, A, A},
        {N, N, N, N, N, A, A, A},
        {N, N, N, N, N, A, A, A},
        {N, N, N, N, N, A, A, A},
        {A, A, N, N, N, A, A, A},
        {A, A, N, N, N, A, A, A},
        {A, A, N, N, N, A, A, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Letra K */
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, A, A, N, N},
        {A, A, N, A, A, N, N, N},
        {A, A, A, A, N, N, N, N},
        {A, A, A, A, N, N, N, N},
        {A, A, N, A, A, N, N, N},
        {A, A, N, N, A, A, N, N},
        {A, A, N, N, N, A, A, N}
    },
    {   /* Letra L */
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Letra M */
        {A, A, N, N, N, N, A, A},
        {A, A, A, N, N, A, A, A},
        {A, A, N, A, A, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra N */
        {A, A, N, N, N, N, A, A},
        {A, A, A, N, N, N, A, A},
        {A, A, N, A, N, N, A, A},
        {A, A, N, N, A, N, A, A},
        {A, A, N, N, N, A, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra O */
        {N, A, A, A, A, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Letra P */
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N}
    },
    {   /* Letra Q */
        {N, A, A, A, A, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, A, N, A, A},
        {A, A, N, N, N, A, A, A},
        {A, A, N, N, N, N, A, A},
        {N, A, A, A, A, A, A, A}
    },
    {   /* Letra R */
        {A, A, A, A, A, A, N, N},
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, N, N, A, A},
        {A, A, A, A, A, A, N, N},
        {A, A, N, A, A, N, N, N},
        {A, A, N, N, A, A, N, N},
        {A, A, N, N, N, A, A, N},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra S */
        {N, A, A, A, A, A, A, A},
        {A, A, N, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {N, A, A, A, A, A, A, N},
        {N, N, N, N, N, N, A, A},
        {N, N, N, N, N, N, A, A},
        {N, N, N, N, N, N, A, A},
        {A, A, A, A, A, A, A, N}
    },
    {   /* Letra T */
        {A, A, A, A, A, A, A, A},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N}
    },
    {   /* Letra U */
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Letra V */
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {N, A, A, N, N, A, A, N},
        {N, A, A, N, N, A, A, N},
        {N, N, A, A, A, A, N, N},
        {N, N, A, A, A, A, N, N},
        {N, N, N, A, A, N, N, N}
    },
    {   /* Letra W */
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, N, N, N, A, A},
        {A, A, N, A, A, N, A, A},
        {A, A, N, A, A, N, A, A},
        {A, A, A, N, N, A, A, A},
        {A, A, A, N, N, A, A, A},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra X */
        {A, A, N, N, N, N, A, A},
        {N, A, A, N, N, A, A, N},
        {N, N, A, A,A, A, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, A, A, A, A, N, N},
        {N, A, A, N, N, A, A, N},
        {A, A, N, N, N, N, A, A}
    },
    {   /* Letra Y */
        {A, A, N, N, N, N, A, A},
        {N, A, A, N, N, A, A, N},
        {N, N, A, A, A, A, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N}
    },
    {   /* Letra Z */
        {A, A, A, A, A, A, A, A},
        {N, N, N, N, N, A, A, N},
        {N, N, N, N, A, A, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, A, A, N, N, N, N},
        {N, A, A, N, N, N, N, N},
        {A, A, N, N, N, N, N, N},
        {A, A, A, A, A, A, A, A}
    }
};

/* Se plasman los numeros del 0 al 9 en matrices de 8x8 */
static const uint8_t fuenteNumeros[CANT_NUMEROS][8][PIXELES_X_LADO] =
{
    {   /* Numero 0 */
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Numero 1 */
        {N, N, N, A, A, N, N, N},
        {N, N, A, A, A, N, N, N},
        {N, A, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, N, A, A, N, N, N},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Numero 2 */
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, A, N},
        {N, N, N, N, N, A, N, N},
        {N, N, N, N, A, N, N, N},
        {N, N, N, A, N, N, N, N},
        {A, A, A, A, A, A, A, A}
    },
    {   /* Numero 3 */
        {A, A, A, A, A, A, A, N},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, A, A, A, N},
        {N, N, N, A, A, A, A, N},
        {N, N, N, N, A, A, A, N},
        {N, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Numero 4 */
        {N, N, N, N, A, A, N, N},
        {N, N, N, A, A, A, N, N},
        {N, N, A, N, A, A, N, N},
        {N, A, N, N, A, A, N, N},
        {A, A, A, A, A, A, A, A},
        {N, N, N, N, A, A, N, N},
        {N, N, N, N, A, A, N, N},
        {N, N, N, N, A, A, N, N}
    },
    {   /* Numero 5 */
        {A, A, A, A, A, A, A, A},
        {A, N, N, N, N, N, N, N},
        {A, A, A, A, A, A, A, N},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Numero 6 */
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, N},
        {A, N, N, N, N, N, N, N},
        {A, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Numero 7 */
        {A, A, A, A, A, A, A, A},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, A, N},
        {N, N, N, N, N, A, N, N},
        {N, N, N, N, A, N, N, N},
        {N, N, N, A, N, N, N, N},
        {N, N, A, N, N, N, N, N},
        {N, A, N, N, N, N, N, N}
    },
    {   /* Numero 8 */
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
    },
    {   /* Numero 9 */
        {N, A, A, A, A, A, A, N},
        {A, N, N, N, N, N, N, A},
        {A, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, A},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, N, A},
        {N, N, N, N, N, N, N, A},
        {N, A, A, A, A, A, A, N}
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
