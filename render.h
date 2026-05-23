#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED
#include "alfabeto.h"
#include "tetris.h"

void obtener_resolucion_logica(int resolucion, int* ancho, int* alto);
void recrear_ventana(Configuracion* config);
void aplicar_paleta(int tipo_paleta);
//void render_pantalla(t_tetris* juego, Configuracion* config, int opcion_menu);

void render_pantalla(t_tetris* juego, Configuracion* config, int opcion_menu, const Alfabeto* alf, int ancho_logico, int alto_logico);
#endif // RENDER_H_INCLUDED
