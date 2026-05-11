/*
Apellido: Mendoza, Micaela Belen
DNI: 45778189
Usuario: mmendoza189unlam
Entrega: Sí

Apellido: Vega, Villalba Ariel
DNI: 32865933
Usuario: arielvegav13
Entrega: Sí

Apellido: Aguirre, Camila Luciana
DNI: 46183931
Usuario: Cami-2901
Entrega: Sí

*/
#include <stdio.h>
#include <stdlib.h>
#include "dibujos.h"
#include "tetris.h"
#include <time.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "GBT/gbt.h"
#include "presentacion.h"
#define PARAMETROS_TOTAL 3
#define CANT_COLORES 16
#define TAM_GRILLA 11
#define EXITO 0
#define ERROR 1

int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana);

tGBT_ColorRGB paletaCGA[CANT_COLORES] = {

    /// 0-15: Colores CGA (16 colores)
    {0x00, 0x00, 0x00}, // 0:   Negro
    {0x00, 0x00, 0xAA}, // 1:   Azul
    {0x00, 0xAA, 0x00}, // 2:   Verde
    {0x00, 0xAA, 0xAA}, // 3:   Cian
    {0xAA, 0x00, 0x00}, // 4:   Rojo
    {0xAA, 0x00, 0xAA}, // 5:   Magenta
    {0xAA, 0x55, 0x00}, // 6:   Marron
    {0xAA, 0xAA, 0xAA}, // 7:   Gris claro
    {0x55, 0x55, 0x55}, // 8:   Gris oscuro
    {0x55, 0x55, 0xFF}, // 9:   Azul brillante
    {0x55, 0xFF, 0x55}, // 10:  Verde brillante
    {0x55, 0xFF, 0xFF}, // 11:  Cian brillante
    {0xFF, 0x55, 0x55}, // 12:  Rojo brillante
    {0xFF, 0x55, 0xFF}, // 13:  Magenta brillante
    {0xFF, 0xFF, 0x55}, // 14:  Amarillo
    {0xFF, 0xFF, 0xFF}  // 15:  Usado como transparente por GBT
};

int main(int argc, char* argv[])
{
    int ancho_ventana, alto_ventana, escala_ventana;
    int rta;
    rta = leerArgumentos(argc, argv, &ancho_ventana, &alto_ventana, &escala_ventana);
    if(rta == ERROR)
        return ERROR;

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d", ancho_ventana, alto_ventana);

    if (gbt_crear_ventana(nombreVentana, ancho_ventana, alto_ventana, escala_ventana) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        return -1;
    }

    tGBT_Temporizador *temporizador = gbt_temporizador_crear(1.0);
    if (!temporizador) {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        return -1;
    }

    srand(time(0));

    Alfabeto alf;
    alfabetoCrear(&alf);

    if(!presentacionEjecutar(&alf, ancho_ventana, alto_ventana))
    {
        gbt_destruir_ventana();
        gbt_cerrar();
        return ERROR;
    }

    char nombreJugador[USERNAME_MAX_LEN + 1];
    if(!usernameEjecutar(&alf, nombreJugador, ancho_ventana, alto_ventana))
    {
        gbt_destruir_ventana();
        gbt_cerrar();
        return ERROR;
    }
    tetrisEjecutar(&alf,nombreJugador,ancho_ventana,alto_ventana);

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();
    return 0;
}

int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana)
{
    // 1. cantidad de parametros
    if(argc != PARAMETROS_TOTAL)
    {
        printf("ERROR 1: La cantidad de parametros no es la esperada\n");
        printf("Uso: tetris.exe [CGA|VGA] [1-4]\n");
        printf("Ejemplo: tetris.exe CGA 3");
        return ERROR;
    }
    char resolucion[4];
    char escala[2];

    strcpy(resolucion, argv[1]);
    strcpy(escala, argv[2]);

    // 2. largo de resolucion
    if(strlen(resolucion) != 3)
    {
        printf("ERROR: Resolucion invalida\n");
        printf("Resoluciones validas: CGA, VGA\n");
        return ERROR;
    }

    // 3. resolucion valida
    if(strcmpi(resolucion, "VGA") != 0 && strcmpi(resolucion, "CGA") != 0)
    {
        printf("ERROR: Resolucion invalida\n");
        printf("Resoluciones validas: CGA, VGA\n");
        return ERROR;
    }

    // 4. escala es digito
    if(!isdigit(escala[0]))
    {
        printf("ERROR: Escala invalida\n");
        printf("La escala debe ser un numero entre 1 y 4\n");
        return ERROR;
    }

    // 5. escala en rango
    if(escala[0] < '1' || escala[0] > '4')
    {
        printf("ERROR: Escala invalida\n");
        printf("La escala debe ser un numero entre 1 y 4\n");
        return ERROR;
    }

    // 6. escala valida segun resolucion
    if(strcmpi(resolucion, "VGA") == 0 && escala[0] > '2')
    {
        printf("ERROR: Para VGA la escala debe ser 1 o 2\n");
        printf("Con escala mayor la ventana excede el tamanio del monitor\n");
        return ERROR;
    }

    if(strcmpi(resolucion, "VGA") == 0 )
    {
        *ancho = 640;
        *alto = 480;
        printf("se elegio vga");
    } else{
        *ancho = 320;
        *alto = 200;
        printf("se elegio cga");
    }

    *escala_ventana = escala[0] - '0';
    return EXITO;
}
