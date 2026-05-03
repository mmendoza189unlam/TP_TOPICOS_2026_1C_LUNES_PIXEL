#include <stdio.h>
#include <stdlib.h>
#include "dibujos.h"
#include <time.h>
#include <stdint.h>
#include "GBT/gbt.h"

#define ANCHO_VENTANA 320
#define ALTO_VENTANA 200
#define ESCALA_VENTANA 3
#define CANT_COLORES 16
#define TAM_GRILLA 11

const uint8_t letra_A[8][8] ={
{N,N,A,A,A,A,N,N},
{N,A,N,N,N,N,A,N},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,A,A,A,A,A,A,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A}
};

const uint8_t letra_E[8][8] ={
{A,A,A,A,A,A,A,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,A,A,A,A,A,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,A,A,A,A,A,A,N}
};

const uint8_t letra_L[8][8] = {
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,A,A,A,A,A,A,A}
};
const uint8_t letra_U[8][8] ={
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{N,A,A,A,A,A,A,N},
{N,N,N,N,N,N,N,N}
};

const uint8_t letra_N[8][8] ={
{A,N,N,N,N,N,N,A},
{A,A,N,N,N,N,N,A},
{A,N,A,N,N,N,N,A},
{A,N,N,A,N,N,N,A},
{A,N,N,N,A,N,N,A},
{A,N,N,N,N,A,N,A},
{A,N,N,N,N,N,A,A},
{A,N,N,N,N,N,N,A}
};

const uint8_t letra_M[8][8] = {
{A,N,N,N,N,N,N,A},
{A,A,N,N,N,N,A,A},
{A,N,A,N,N,A,N,A},
{A,N,N,A,A,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A}
};

const uint8_t letra_T[8][8] = {
{A,A,A,A,A,A,A,A},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N}
};

const uint8_t letra_R[8][8] = {
{A,A,A,A,A,A,A,N},
{A,N,N,N,N,N,A,N},
{A,N,N,N,N,N,A,N},
{A,A,A,A,A,A,A,N},
{A,N,N,A,N,N,N,N},
{A,N,N,N,A,N,N,N},
{A,N,N,N,N,A,N,N},
{A,N,N,N,N,N,A,N}
};

const uint8_t letra_I[8][8] = {
{A,A,A,A,A,A,A,A},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{A,A,A,A,A,A,A,A}
};

const uint8_t letra_S[8][8] = {
{A,A,A,A,A,A,A,A},
{A,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,N},
{A,A,A,A,A,A,A,A},
{N,N,N,N,N,N,N,A},
{N,N,N,N,N,N,N,A},
{A,N,N,N,N,N,N,A},
{A,A,A,A,A,A,A,A}
};

const uint8_t letra_P[8][8] ={
{A,A,A,A,A,A,A,N},
{A,N,N,N,N,N,A,N},
{A,N,N,N,N,N,A,N},
{A,A,A,A,A,A,A,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N},
{A,N,N,N,N,N,N,N}
};

const uint8_t letra_X[8][8] = {
{A,N,N,N,N,N,N,A},
{N,A,N,N,N,N,A,N},
{N,N,A,N,N,A,N,N},
{N,N,N,A,A,N,N,N},
{N,N,N,A,A,N,N,N},
{N,N,A,N,N,A,N,N},
{N,A,N,N,N,N,A,N},
{A,N,N,N,N,N,N,A}
};


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
    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d", ANCHO_VENTANA, ALTO_VENTANA);

    if (gbt_crear_ventana(nombreVentana, ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0) {
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

    // Patron de fondo fijo - definido ANTES del while
    uint8_t patron[25][40];
    uint8_t coloresFondo[] = {9, 11, 12, 13, 14, 10};
    for(int f = 0; f < 25; f++)
        for(int c = 0; c < 40; c++)
            patron[f][c] = coloresFondo[(f + c) % 6];

    uint8_t corriendo = 1;
    uint8_t mostrarPress = 1;

    while (corriendo) {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE) {
            corriendo = 0;
        } else if (tecla == GBTK_ENTER) {
            gbt_borrar_backbuffer(0);
            gbt_volcar_backbuffer();
            gbt_esperar(500);
        }

        if (gbt_temporizador_consumir(temporizador)) {
            mostrarPress = !mostrarPress;
        }

        // Fondo fijo
        for(int f = 0; f < 25; f++)
            for(int c = 0; c < 40; c++)
                for(int py = 0; py < 8; py++)
                    for(int px = 0; px < 8; px++)
                        gbt_dibujar_pixel(c*8+px, f*8+py, patron[f][c]);

        // Recuadro negro central
        int recX = 24;
        int recY = 24;
        int recW = 272;
        int recH = 152;
        for(int py = 0; py < recH; py++)
            for(int px = 0; px < recW; px++)
                gbt_dibujar_pixel(recX+px, recY+py, 0);

        // TETRIS PIXEL
        dibujar(letra_T, 7,  5);
        dibujar(letra_E, 8,  5);
        dibujar(letra_T, 9,  5);
        dibujar(letra_R, 10, 5);
        dibujar(letra_I, 11, 5);
        dibujar(letra_S, 12, 5);
        dibujar(letra_P, 14, 5);
        dibujar(letra_I, 15, 5);
        dibujar(letra_X, 16, 5);
        dibujar(letra_E, 17, 5);
        dibujar(letra_L, 18, 5);

        // UNLAM
        dibujar(letra_U, 10, 7);
        dibujar(letra_N, 11, 7);
        dibujar(letra_L, 12, 7);
        dibujar(letra_A, 13, 7);
        dibujar(letra_M, 14, 7);

        // PRESS START
        if (mostrarPress) {
            dibujar(letra_P, 7,  10);
            dibujar(letra_R, 8,  10);
            dibujar(letra_E, 9,  10);
            dibujar(letra_S, 10, 10);
            dibujar(letra_S, 11, 10);
            dibujar(letra_S, 13, 10);
            dibujar(letra_T, 14, 10);
            dibujar(letra_A, 15, 10);
            dibujar(letra_R, 16, 10);
            dibujar(letra_T, 17, 10);
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();
    return 0;
}
