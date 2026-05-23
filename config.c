#include "config.h"
#include "graficos.h"
#include "tetris.h"
#include "GBT/gbt.h"
#include <stdio.h>

#define ARCHIVO_CONFIG "config.dat"

tGBT_ColorRGB paleta[9] = {
    {  0,   0,   0},   // 0 negro/transparente
    {  0, 255, 255},   // 1 celeste  (C)
    {255, 255,   0},   // 2 amarillo (A)
    {  0, 255,   0},   // 3 verde    (V)
    {255,   0,   0},   // 4 rojo
    {  0,   0, 255},   // 5 azul
    {255, 140,   0},   // 6 naranja
    {180,   0, 255},   // 7 violeta
    {  0,   0,   0},   // 8 reservado GBT
};

/* ----- Variable global ----- */
Configuracion config;

/* ----- Variables de dibujo (definidas en render.c) ----- */
extern int escala_dibujo;
extern int offsetX;
extern int offsetY;

/* ----------------------------------------------
obtener_resolucion_logica: traduce el enum
ResolucionLogica a píxeles concretos.
Recibe punteros de sald9ia porque C no puede
retornar dos valores
---------------------------------------------- */
void obtener_resolucion_logica(int resolucion, int *ancho, int *alto)
{
    if(resolucion == RES_CGA){
        *ancho =320;
        *alto = 200;
    }
    else {
        *ancho = 640;
        *alto = 480;
    }
}

/* ----------------------------------------------
recrear_ventana: destruye la ventana actual
y la recrea con la resolucion y escala del
config vigente. Recalcula escala dibujo y
los offsets para centrar el tablero.
---------------------------------------------- */

void recrear_ventana(void)
{
    int ancho_logico, alto_logico;
    obtener_resolucion_logica(config.resolucion, &ancho_logico, &alto_logico);

    escala_dibujo = (config.resolucion == RES_CGA) ? 1 : 2;

    int ancho_tablero = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
    int alto_tablero  = ALTO_VISIBLE  * PIXELES_X_LADO * escala_dibujo;
    int ancho_panel   = 80;

    offsetX = (ancho_logico - (ancho_tablero + ancho_panel)) / 2;
    offsetY = (alto_logico  -  alto_tablero)                 / 2;

    gbt_destruir_ventana();
    gbt_crear_ventana("Tetris", ancho_logico, alto_logico, config.escala);
}

/* ----------------------------------------------
guardar_config: persiste el struct en binario.
---------------------------------------------- */
void guardar_config(void)
{
    FILE *f = fopen(ARCHIVO_CONFIG, "wb");
    if(f){
        fwrite(&config, sizeof(Configuracion), 1, f);
        fclose(f);
    }
}

/* ----------------------------------------------
cargar_config: lee config desde archivo.
Si no existe o tiene valores inválidos,
aplica valores por defecto.
---------------------------------------------- */
void cargar_config(void)
{
    FILE *f = fopen(ARCHIVO_CONFIG, "rb");
    if (f) {
        fread(&config, sizeof(Configuracion), 1, f);
        fclose(f);

        /* validaciones por si el archivo está corrupto */
        if (config.resolucion != RES_CGA && config.resolucion != RES_VGA)
            config.resolucion = RES_CGA;
        if (config.escala < 1 || config.escala > 6)
            config.escala = 2;
        if (config.velocidad < 0.1f || config.velocidad > 1.0f)
            config.velocidad = 0.5f;
        if (config.paleta < 0 || config.paleta > 1)
            config.paleta = 0;
    } else {
        /* primera ejecución: valores por defecto */
        config.paleta     = 0;
        config.resolucion = RES_CGA;
        config.escala     = 2;
        config.velocidad  = 0.5f;
    }
}
