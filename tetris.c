/*
 tetris.c - Motor lógico del juego.
 Implementa la mecánica de las piezas, colisiones, rotaciones y gestión
 del tablero (limpieza de líneas y puntuación).
 */

#include "tetris.h"
#include <stdlib.h>
#include <string.h>

// Definición de las formas base de cada pieza (tetrominós)
static const uint8_t piezas_orig[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}
};

// Algoritmo de Fisher-Yates para asegurar piezas aleatorias sin repetición inmediata
static void mezclar_bag(t_tetris* j) {
    for (int i = 0; i < 7; i++) {
        j->bag[i] = i;
    }
    for (int i = 6; i > 0; i--) {
        int rnd = rand() % (i + 1);
        int t = j->bag[i];
        j->bag[i] = j->bag[rnd];
        j->bag[rnd] = t;
    }
    j->bag_index = 0;
}

static int siguiente_pieza(t_tetris* j) {
    if (j->bag_index >= 7) {
        mezclar_bag(j);
    }
    return j->bag[j->bag_index++];
}

// Verifica si la pieza actual colisiona con límites del tablero o bloques fijos
static int colisiona(t_tetris* j, uint8_t p[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            if (p[i][k]) {
                int tx = x + k, ty = y + i;
                if (tx < 0 || tx >= ANCHO_TABLERO || ty >= ALTO_TABLERO) {
                    return 1;
                }
                if (ty >=0 && j->tablero[ty][tx]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Rotación de matriz 4x4 (sentido horario)
static void rotar(uint8_t out[4][4], uint8_t in[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            out[k][3 - i] = in[i][k];
        }
    }
}

// Rotación de matriz 4x4 (sentido antihorario)
static void rotar_izquierda(uint8_t out[4][4], uint8_t in[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            out[3 - k][i] = in[i][k];
        }
    }
}

// Fija la pieza actual en el tablero una vez que no puede bajar más
static void fijar(t_tetris* j) {
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            if (j->piezas[j->pieza_actual][i][k]) {
                j->tablero[j->posY + i][j->posX + k] = j->pieza_actual + 1;
            }
        }
    }
}

// Verifica líneas completas, las elimina y desplaza las superiores
static void limpiar(t_tetris* j) {
    int filas = 0;

    for (int y = 0; y < ALTO_TABLERO; y++) {
        int llena = 1;
        for (int x = 0; x < ANCHO_TABLERO; x++) {
            if (!j->tablero[y][x]) {
                llena = 0;
                break; // Si hay un espacio vacío, no está llena
            }
        }
        if (llena) {
            // Guardamos la dirección de memoria de la fila que se completó
            uint8_t* fila_temporal = j->tablero[y];

            // Desplazamos únicamente los PUNTEROS de las filas superiores hacia abajo
            for (int yy = y; yy > 0; yy--) {
                j->tablero[yy] = j->tablero[yy - 1];
            }

            // El puntero de la fila eliminada se convierte ahora en la nueva fila superior (fila 0)
            j->tablero[0] = fila_temporal;

            // Limpiamos los elementos de esta nueva fila superior poniéndolos en 0
            for (int x = 0; x < ANCHO_TABLERO; x++) {
                j->tablero[0][x] = 0;
            }

            filas++;
            j->lineas_totales ++; //Suma al acumulador global de lineas
        }
    }
    int tabla[] = {0, 100, 300, 500, 800};
    if (filas > 0) {
        int multiplicador = (j->nivel + 1);
        int bono_velocidad = j->nivel * 5;
        j->puntaje += (tabla[filas] * multiplicador) + bono_velocidad;
    }
}
//Actualiza el multiplicador de velocidad dinamico cada 10 piezas fijadas
static void actualizar_dificultad(t_tetris* j) {
    j->contador_fichas++;
    if (j->contador_fichas % 10 == 0) {
        j->intervalo_actual *= 0.97f; // Acelera un 3% la caída
        j->nivel=j->contador_fichas / 10;
    }
}

// Genera una nueva pieza en el tablero
void tetris_nueva(t_tetris* j) {
    j->pieza_actual = j->siguiente;
    j->siguiente = siguiente_pieza(j);

    // Suma al panel estadístico de piezas usadas
    j->cant_piezas_usadas[j->pieza_actual]++;

    memcpy(j->piezas[j->pieza_actual], piezas_orig[j->pieza_actual], sizeof(j->piezas[0]));
    j->posX = (ANCHO_TABLERO / 2) - 2; // Centrado exacto
    j->posY = 0;

    // Si al nacer ya colisiona, Game Over directo sin colgar el bucle
    if (colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) {
        j->estado = ESTADO_GAMEOVER;
    }
}

// Inicializa el tablero y variables de estado para una nueva partida
void tetris_reiniciar(t_tetris* j) {

    // Primero liberamos memoria si es que ya había sido asignada en una partida anterior
    for (int y = 0; y < ALTO_TABLERO; y++) {
        if (j->tablero[y] != NULL) {
            free(j->tablero[y]);
            j->tablero[y] = NULL;
        }
    }

    // Asignamos memoria dinámica para cada una de las filas
    for (int y = 0; y < ALTO_TABLERO; y++) {
        j->tablero[y] = (uint8_t*)malloc(ANCHO_TABLERO * sizeof(uint8_t));
        // Inicializamos la fila en 0 (vacía)
        for (int x = 0; x < ANCHO_TABLERO; x++) {
            j->tablero[y][x] = 0;
        }
    }

    memset(j->cant_piezas_usadas,0,sizeof(j->cant_piezas_usadas));

    j->puntaje = 0;
    j->lineas_totales = 0;
    j->contador_fichas = 0;
    j->nivel=0;
    j->intervalo_actual = 1000.0f; // Reset de milisegundos base
    j->game_over = 0;
    j->estado = ESTADO_JUGANDO;

    memcpy(j->piezas, piezas_orig, sizeof(piezas_orig));
    mezclar_bag(j);
    j->siguiente = siguiente_pieza(j);

    j->en_espera_fijacion = 0;
    if (j->timer_fijacion) {
        gbt_temporizador_destruir(j->timer_fijacion);
        j->timer_fijacion = NULL;
    }

    tetris_nueva(j);
}

//Reinicio de la fijacion
void tetris_intentar_reset_fijacion(t_tetris* j) {
    if (j->en_espera_fijacion) {
        j->en_espera_fijacion = 0;
        if (j->timer_fijacion) {
            gbt_temporizador_destruir(j->timer_fijacion);
            j->timer_fijacion = NULL;
        }
    }
}

void tetris_rotar_der(t_tetris* j) {
    uint8_t tmp[4][4];
    rotar(tmp, j->piezas[j->pieza_actual]);
    if (!colisiona(j, tmp, j->posX, j->posY)) {
        memcpy(j->piezas[j->pieza_actual], tmp, sizeof(tmp));
        tetris_intentar_reset_fijacion(j);
    }
}

void tetris_rotar_izq(t_tetris* j) {
    uint8_t tmp[4][4];
    rotar_izquierda(tmp, j->piezas[j->pieza_actual]);
    if (!colisiona(j, tmp, j->posX, j->posY)) {
        memcpy(j->piezas[j->pieza_actual], tmp, sizeof(tmp));
        tetris_intentar_reset_fijacion(j);
    }
}

void tetris_mover_izq(t_tetris* j) {
    j->posX--;
    if (colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) {
        j->posX++;
    }else {
        tetris_intentar_reset_fijacion(j);
    }
}

void tetris_mover_der(t_tetris* j) {
    j->posX++;
    if (colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) {
        j->posX--;
    }else {
        tetris_intentar_reset_fijacion(j);
    }
}

//Baja la pieza con la fecha de hacia abajo
int tetris_mover_abajo_manual(t_tetris* j) {
    j->posY++;
    if (colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) {
        j->posY--;
        return 0; // Colisionó abajo
    } else {
        j->puntaje += 1 + (j->nivel * 5);
        tetris_intentar_reset_fijacion(j);
        return 1; // Se movió con éxito
    }
}


// Desplazamiento rápido de la pieza hasta el fondo del tablero
void tetris_hard_drop(t_tetris* j) {
    int celdas = 0;
    while (!colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY + 1)) {
        j->posY++;
        celdas++;
    }
    j->puntaje += celdas * (2 + j->nivel);

    fijar(j);
    limpiar(j);
    actualizar_dificultad(j);
    tetris_intentar_reset_fijacion(j);
    tetris_nueva(j);
}
//Avance de las piezas por gravedad
void tetris_procesar_gravedad(t_tetris* j, tGBT_Temporizador* timer_caida) {
    // 1. Procesar caída automática del temporizador global
    if (gbt_temporizador_consumir(timer_caida)) {
        if (!colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY + 1)) {
            j->posY++;
            tetris_intentar_reset_fijacion(j);
        } else {
            // Si choca contra el suelo y no tiene el Lock Delay activo, se inicia el contador
            if (!j->en_espera_fijacion) {
                j->en_espera_fijacion = 1;
                j->timer_fijacion = gbt_temporizador_crear(TIEMPO_GRACIA);
            }
        }
    }

    // 2. Comprobar si el tiempo de gracia del Lock Delay se agotó
    if (j->en_espera_fijacion && j->timer_fijacion != NULL) {
        if (gbt_temporizador_consumir(j->timer_fijacion)) {
            fijar(j);
            limpiar(j);
            actualizar_dificultad(j);

            // Apagar timer de gracia actual
            j->en_espera_fijacion = 0;
            gbt_temporizador_destruir(j->timer_fijacion);
            j->timer_fijacion = NULL;

            tetris_nueva(j);
        }
    }

}

