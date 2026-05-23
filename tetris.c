#include "tetris.h"
#include <stdlib.h>
#include <string.h>

// Las piezas originales se mudan acá y se vuelven privadas (static)
static const uint8_t piezas_orig[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}
};

static void mezclar_bag(t_tetris* j){
    for(int i=0;i<7;i++) j->bag[i]=i;
    for(int i=6;i>0;i--){
        int rnd=rand()%(i+1);
        int t=j->bag[i]; j->bag[i]=j->bag[rnd]; j->bag[rnd]=t;
    }
    j->bag_index=0;
}

static int siguiente_pieza(t_tetris* j){
    if(j->bag_index>=7) mezclar_bag(j);
    return j->bag[j->bag_index++];
}

static int colisiona(t_tetris* j, uint8_t p[4][4], int x, int y){
    for(int i=0;i<4;i++)
        for(int k=0;k<4;k++)
            if(p[i][k]){
                int tx=x+k, ty=y+i;
                if(tx<0||tx>=ANCHO_TABLERO||ty>=ALTO_TABLERO) return 1;
                if(ty>=0 && j->tablero[ty][tx]) return 1;
            }
    return 0;
}

static void rotar(uint8_t out[4][4], uint8_t in[4][4]){
    for(int i=0;i<4;i++)
        for(int k=0;k<4;k++)
            out[k][3-i]=in[i][k];
}

static void rotar_izquierda(uint8_t out[4][4], uint8_t in[4][4]){
    for(int i=0;i<4;i++)
        for(int k=0;k<4;k++)
            out[3-k][i]=in[i][k];
}

static void fijar(t_tetris* j){
    for(int i=0;i<4;i++)
        for(int k=0;k<4;k++)
            if(j->piezas[j->pieza_actual][i][k])
                j->tablero[j->posY+i][j->posX+k]=j->pieza_actual+1;
}

static void limpiar(t_tetris* j){
    int filas = 0;
    for(int y=0;y<ALTO_TABLERO;y++){
        int llena=1;
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(!j->tablero[y][x]) llena=0;
        if(llena){
            for(int yy=y;yy>0;yy--)
                for(int x=0;x<ANCHO_TABLERO;x++)
                    j->tablero[yy][x]=j->tablero[yy-1][x];
            y--;
            filas++;
        }
    }
    int tabla[] = {0, 100, 300, 500, 800};
    if(filas > 0) j->puntaje += tabla[filas];
}

void tetris_nueva(t_tetris* j){
    j->pieza_actual = j->siguiente;
    j->siguiente = siguiente_pieza(j);
    memcpy(j->piezas[j->pieza_actual], piezas_orig[j->pieza_actual], sizeof(j->piezas[0]));
    j->posX = 3;
    j->posY = 0;
    if(colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)){
        j->estado = ESTADO_GAMEOVER;
    }
}

void tetris_reiniciar(t_tetris* j){
    memset(j->tablero, 0, sizeof(j->tablero));
    j->puntaje = 0;
    j->game_over = 0;
    j->estado = ESTADO_JUGANDO;
    memcpy(j->piezas, piezas_orig, sizeof(piezas_orig));
    mezclar_bag(j);
    j->siguiente = siguiente_pieza(j);
    tetris_nueva(j);
}

void tetris_rotar_der(t_tetris* j){
    uint8_t tmp[4][4];
    rotar(tmp, j->piezas[j->pieza_actual]);
    if(!colisiona(j, tmp, j->posX, j->posY))
        for(int i=0;i<4;i++)
            for(int k=0;k<4;k++)
                j->piezas[j->pieza_actual][i][k]=tmp[i][k];
}

void tetris_rotar_izq(t_tetris* j){
    uint8_t tmp[4][4];
    rotar_izquierda(tmp, j->piezas[j->pieza_actual]);
    if(!colisiona(j, tmp, j->posX, j->posY))
        for(int i=0;i<4;i++)
            for(int k=0;k<4;k++)
                j->piezas[j->pieza_actual][i][k]=tmp[i][k];
}

void tetris_mover_izq(t_tetris* j){
    j->posX--;
    if(colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) j->posX++;
}

void tetris_mover_der(t_tetris* j){
    j->posX++;
    if(colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)) j->posX--;
}

void tetris_mover_abajo(t_tetris* j){
    j->posY++;
    if(colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)){
        j->posY--;
    } else {
        j->puntaje += 1;
    }
}

void tetris_hard_drop(t_tetris* j){
    while(!colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)){
        j->posY++;
        j->puntaje += 2;
    }
    j->posY--;
    fijar(j);
    limpiar(j);
    tetris_nueva(j);
}

void tetris_tick(t_tetris* j){
    j->posY++;
    if(colisiona(j, j->piezas[j->pieza_actual], j->posX, j->posY)){
        j->posY--;
        fijar(j);
        limpiar(j);
        tetris_nueva(j);
    }
}
