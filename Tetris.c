#include "tetris.h"
#include "dibujos.h"
#include "GBT/gbt.h"
#include <stdlib.h>
#include <string.h>

static const uint8_t bloque[CANT_PIEZAS][8][8] = {
    {{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},
     {C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},
     {A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}},
    {{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},
     {V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},
     {A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}},
    {{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},
     {C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C}},
    {{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},
     {V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},
     {A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}}
};

static const uint8_t piezas_orig[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}  // L
};

uint8_t tablero[ALTO_TABLERO][ANCHO_TABLERO];
uint8_t piezas[CANT_PIEZAS][4][4];
int game_over;
int puntaje;
int pieza_actual, posX, posY;
int siguiente;
int bag[7], bag_index;

int offsetCol;
int offsetFil;

// ── Funciones privadas ────────────────────────────────────────

void mezclar_bag(void)
{
    int i, j, t;
    for (i = 0; i < 7; i++) bag[i] = i;
    for (i = 6; i > 0; i--) {
        j = rand() % (i + 1);
        t = bag[i]; bag[i] = bag[j]; bag[j] = t;
    }
    bag_index = 0;
}

int siguiente_pieza(void)
{
    if (bag_index >= 7) mezclar_bag();
    return bag[bag_index++];
}

int colisiona(uint8_t p[4][4], int x, int y)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (p[i][j]) {
                int tx = x + j, ty = y + i;
                if (tx < 0 || tx >= ANCHO_TABLERO || ty >= ALTO_TABLERO) return 1;
                if (ty >= 0 && tablero[ty][tx]) return 1;
            }
    return 0;
}

void rotar(uint8_t out[4][4], uint8_t in[4][4])
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            out[j][3 - i] = in[i][j];
}

void intentar_rotar(void)
{
    uint8_t tmp[4][4];
    int i, j;
    rotar(tmp, piezas[pieza_actual]);
    if (!colisiona(tmp, posX, posY))
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                piezas[pieza_actual][i][j] = tmp[i][j];
}

void fijar(void)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (piezas[pieza_actual][i][j])
                tablero[posY + i][posX + j] = pieza_actual + 1;
}

void limpiar(void)
{
    static const int tabla[] = {0, 100, 300, 500, 800};
    int y, yy, x, filas = 0;
    for (y = 0; y < ALTO_TABLERO; y++) {
        int llena = 1;
        for (x = 0; x < ANCHO_TABLERO; x++)
            if (!tablero[y][x]) { llena = 0; break; }
        if (llena) {
            for (yy = y; yy > 0; yy--)
                for (x = 0; x < ANCHO_TABLERO; x++)
                    tablero[yy][x] = tablero[yy - 1][x];
            y--;
            filas++;
        }
    }
    if (filas > 0) puntaje += tabla[filas];
}

void nueva(void)
{
    pieza_actual = siguiente;
    siguiente    = siguiente_pieza();
    memcpy(piezas[pieza_actual], piezas_orig[pieza_actual], sizeof(piezas[0]));
    posX = 3;
    posY = 0;
    if (colisiona(piezas[pieza_actual], posX, posY))
        game_over = 1;
}

void render(void)
{
    int x, y, i, j;

    gbt_borrar_backbuffer(0);

    // Bloques fijados en el tablero
    for (y = FILAS_OCULTAS; y < ALTO_TABLERO; y++)
        for (x = 0; x < ANCHO_TABLERO; x++)
            if (tablero[y][x])
                dibujar(bloque[tablero[y][x] - 1],
                        x + offsetCol,
                        y - FILAS_OCULTAS + offsetFil);

    // Pieza actual
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (piezas[pieza_actual][i][j]) {
                int dy = posY + i - FILAS_OCULTAS;
                if (dy >= 0)
                    dibujar(bloque[pieza_actual],
                            posX + j + offsetCol,
                            dy + offsetFil);
            }

    // Bordes del tablero
    int baseX = offsetCol * PIXELES_X_LADO;
    int baseY = offsetFil * PIXELES_X_LADO;
    int anchoTbl = ANCHO_TABLERO * PIXELES_X_LADO;
    int altoTbl  = ALTO_VISIBLE  * PIXELES_X_LADO;

    for (x = 0; x < anchoTbl; x++) {
        gbt_dibujar_pixel(baseX + x, baseY,              V); // arriba
        gbt_dibujar_pixel(baseX + x, baseY + altoTbl,    V); // abajo
    }
    for (y = 0; y <= altoTbl; y++) {
        gbt_dibujar_pixel(baseX - 1,         baseY + y,  V); // izquierda
        gbt_dibujar_pixel(baseX + anchoTbl,  baseY + y,  V); // derecha
    }

    gbt_volcar_backbuffer();
}

int tetrisEjecutar(const Alfabeto* alf, const char* nombreJugador, int ancho, int alto)
{
    // Calcular offset para centrar el tablero
    offsetCol = (ancho - ANCHO_TABLERO * PIXELES_X_LADO) / 2 / PIXELES_X_LADO;
    offsetFil = (alto  - ALTO_VISIBLE  * PIXELES_X_LADO) / 2 / PIXELES_X_LADO;

    // Inicializar estado
    memset(tablero, 0, sizeof(tablero));
    memcpy(piezas, piezas_orig, sizeof(piezas_orig));
    game_over = 0;
    puntaje   = 0;
    bag_index = 0;

    mezclar_bag();
    siguiente = siguiente_pieza();
    nueva();

    tGBT_Temporizador* timer = gbt_temporizador_crear(0.5);
    int corriendo = 1;

    while (!game_over && corriendo)
    {
        gbt_procesar_entrada();

        if (gbt_tecla_presionada(GBTK_IZQUIERDA)) {
            posX--;
            if (colisiona(piezas[pieza_actual], posX, posY)) posX++;
        }
        if (gbt_tecla_presionada(GBTK_DERECHA)) {
            posX++;
            if (colisiona(piezas[pieza_actual], posX, posY)) posX--;
        }
        if (gbt_tecla_presionada(GBTK_ABAJO))
        {
            posY++;
            if (colisiona(piezas[pieza_actual], posX, posY)) posY--;
            else puntaje++;
        }
        if (gbt_tecla_presionada(GBTK_ARRIBA))
        {
            intentar_rotar();
        }

        if (gbt_tecla_presionada(GBTK_ESPACIO))
        {
            while (!colisiona(piezas[pieza_actual], posX, posY))
            {
                posY++;
                puntaje += 2;
            }
            posY--;
            fijar();
            limpiar();
            nueva();
        }

        if (gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo = 0;
        }

        if (gbt_temporizador_consumir(timer)) {
            posY++;
            if (colisiona(piezas[pieza_actual], posX, posY)) {
                posY--;
                fijar();
                limpiar();
                nueva();
            }
        }

        render();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(timer);
    return game_over ? 0 : 1;
}
