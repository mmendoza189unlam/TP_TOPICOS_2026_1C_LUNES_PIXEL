/*
 tetris.h - Cabecera principal del motor de juego.
 Define las constantes, estructuras de datos del estado del juego
 y las funciones que controlan la lógica del Tetris.
 */

#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <stdint.h>

/* Dimensiones del área de juego y configuración */
#define ANCHO_TABLERO 10
#define ALTO_VISIBLE 20
#define FILAS_OCULTAS 4
#define ALTO_TABLERO (ALTO_VISIBLE + FILAS_OCULTAS)
#define CANT_PIEZAS 7
#define ARCHIVO_CONFIG "config.dat"

// Estados posibles de la máquina de estados global
enum {
    ESTADO_PRESENTACION,
    ESTADO_INGRESO_NOMBRE,
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
};

// Modos de resolución lógica permitidos
typedef enum {
    RES_CGA,
    RES_VGA
} ResolucionLogica;

// Estructura para almacenar las preferencias del usuario
typedef struct {
    int paleta;
    int resolucion;
    int escala;
    float velocidad;
} Configuracion;

// Estructura principal que mantiene el estado completo del juego
typedef struct {
    uint8_t tablero[ALTO_TABLERO][ANCHO_TABLERO]; // Estado actual de la cuadrícula
    uint8_t piezas[CANT_PIEZAS][4][4];            // Representación de las piezas
    int game_over;                                // Bandera de fin de juego
    int puntaje;                                  // Puntuación acumulada
    int pieza_actual;                             // Índice de la pieza en juego
    int posX;                                     // Posición horizontal de la pieza
    int posY;                                     // Posición vertical de la pieza
    int siguiente;                                // Índice de la próxima pieza
    int bag[7];                                   // Buffer para aleatoriedad de piezas
    int bag_index;                                // Posición actual en el bag
    char nombre_jugador[13];                      // Identificación del usuario
    int nombre_len;                               // Longitud del nombre actual
    int mostrar_press;                            // Bandera de parpadeo (interfaz)
    int mostrar_cursor;                           // Bandera de parpadeo (interfaz)
    int estado;                                   // Estado actual de la máquina
} t_tetris;


// Inicializa el tablero, el puntaje y las variables de juego
void tetris_reiniciar(t_tetris* j);

// Prepara una nueva pieza para entrar en juego
void tetris_nueva(t_tetris* j);

// Rotaciones y movimientos de la pieza activa
void tetris_rotar_der(t_tetris* j);
void tetris_rotar_izq(t_tetris* j);
void tetris_mover_izq(t_tetris* j);
void tetris_mover_der(t_tetris* j);
void tetris_mover_abajo(t_tetris* j);

// Desplazamiento instantáneo hasta la base del tablero
void tetris_hard_drop(t_tetris* j);

// Ejecuta un ciclo de gravedad automática
void tetris_tick(t_tetris* j);

#endif // TETRIS_H_INCLUDED
