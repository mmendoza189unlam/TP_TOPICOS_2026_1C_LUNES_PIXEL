/*
    render.h - Cabecera del módulo de renderizado.
    Define la interfaz pública para la gestión de la ventana y el renderizado
    de los diferentes estados del juego.
 */

#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "Alfabeto.h"
#include "tetris.h"

// Calcula las dimensiones lógicas (ancho y alto) basadas en el estándar CGA o VGA.
void obtener_resolucion_logica(int resolucion, int* ancho, int* alto);

// Destruye la ventana actual y crea una nueva con los parámetros de configuración.
// Ajusta los offsets de centrado automáticamente.
void recrear_ventana(Configuracion* config);

//Aplica la paleta de colores al motor gráfico GBT.
void aplicar_paleta(int tipo_paleta);

// Renderiza el estado actual del juego en el backbuffer.
void render_pantalla(t_tetris* juego, Configuracion* config, int opcion_menu, const Alfabeto* alf, int ancho_logico, int alto_logicoconst, const char* mejor_jugador, int max_score);

#endif // RENDER_H_INCLUDED
