#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include <stdint.h>

#define ANCHO_TABLERO 10
#define ALTO_VISIBLE 20
#define FILAS_OCULTAS 4
#define ALTO_TABLERO (ALTO_VISIBLE + FILAS_OCULTAS)
#define CANT_PIEZAS 7
#define ARCHIVO_CONFIG "config.dat"


enum {
    ESTADO_PRESENTACION,
    ESTADO_INGRESO_NOMBRE,
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
};

typedef enum {
    RES_CGA,
    RES_VGA
} ResolucionLogica;

typedef struct {
    int paleta;
    int resolucion;
    int escala;
    float velocidad;
} Configuracion;

typedef struct {
    uint8_t tablero[ALTO_TABLERO][ANCHO_TABLERO];
    uint8_t piezas[CANT_PIEZAS][4][4];
    int game_over;
    int puntaje;
    int pieza_actual;
    int posX;
    int posY;
    int siguiente;
    int bag[7];
    int bag_index;
    char nombre_jugador[13]; // 12 caracteres + el '\0'
    int nombre_len;
    int mostrar_press;
    int mostrar_cursor;
    int estado; // ESTADO_MENU, ESTADO_JUGANDO, etc.

} t_tetris;

// Firmas de las funciones del core (reciben puntero a la estructura)
void tetris_reiniciar(t_tetris* j);
void tetris_nueva(t_tetris* j);
void tetris_rotar_der(t_tetris* j);
void tetris_rotar_izq(t_tetris* j);
void tetris_mover_izq(t_tetris* j);
void tetris_mover_der(t_tetris* j);
void tetris_mover_abajo(t_tetris* j);
void tetris_hard_drop(t_tetris* j);
void tetris_tick(t_tetris* j); // Para la caída automática
#endif // TETRIS_H_INCLUDED
