#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "dibujos.h"
#include "GBT/gbt.h"

#define ANCHO_VENTANA 80
#define ALTO_VENTANA 160
#define ESCALA_VENTANA 5

#define ANCHO_TABLERO 10
#define ALTO_VISIBLE 20
#define FILAS_OCULTAS 4
#define ALTO_TABLERO (ALTO_VISIBLE + FILAS_OCULTAS)

#define CANT_PIEZAS 7

uint8_t tablero[ALTO_TABLERO][ANCHO_TABLERO] = {0};

/// BLOQUES VISUALES
const uint8_t bloque[7][8][8] = {
    {{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}},
    {{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}},
    {{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C},{C,C,C,C,C,C,C,C}},
    {{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V},{V,V,V,V,V,V,V,V}},
    {{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A},{A,A,A,A,A,A,A,A}}
};

uint8_t piezas[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}
};

int pieza_actual, posX, posY;

int bag[7], bag_index=0;

void mezclar_bag(){
    for(int i=0;i<7;i++) bag[i]=i;
    for(int i=0;i<7;i++){int j=rand()%7;int t=bag[i];bag[i]=bag[j];bag[j]=t;}
    bag_index=0;
}

int siguiente_pieza(){
    if(bag_index>=7) mezclar_bag();
    return bag[bag_index++];
}

int colisiona(uint8_t p[4][4], int x, int y){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(p[i][j]){
                int tx=x+j, ty=y+i;
                if(tx<0||tx>=ANCHO_TABLERO||ty>=ALTO_TABLERO) return 1;
                if(ty>=0 && tablero[ty][tx]) return 1;
            }
    return 0;
}

void rotar(uint8_t out[4][4], uint8_t in[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            out[j][3-i]=in[i][j];
}

void intentar_rotar(){
    uint8_t tmp[4][4];
    rotar(tmp, piezas[pieza_actual]);
    if(!colisiona(tmp,posX,posY))
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                piezas[pieza_actual][i][j]=tmp[i][j];
}

void fijar(){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(piezas[pieza_actual][i][j])
                tablero[posY+i][posX+j]=pieza_actual+1;
}

void limpiar(){
    for(int y=0;y<ALTO_TABLERO;y++){
        int llena=1;
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(!tablero[y][x]) llena=0;
        if(llena){
            for(int yy=y;yy>0;yy--)
                for(int x=0;x<ANCHO_TABLERO;x++)
                    tablero[yy][x]=tablero[yy-1][x];
        }
    }
}

void nueva(){
    pieza_actual=siguiente_pieza();
    posX=3; posY=0;
    if(colisiona(piezas[pieza_actual],posX,posY)){
        printf("GAME OVER\n");
        exit(0);
    }
}

void render(){
    gbt_borrar_backbuffer(0);

    for(int y=FILAS_OCULTAS;y<ALTO_TABLERO;y++)
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(tablero[y][x])
                dibujar(bloque[tablero[y][x]-1],x,y-FILAS_OCULTAS);

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(piezas[pieza_actual][i][j]){
                int dy=posY+i-FILAS_OCULTAS;
                if(dy>=0)
                    dibujar(bloque[pieza_actual],posX+j,dy);
            }
}

int main(){
    srand(time(0));
    gbt_iniciar();
    gbt_crear_ventana("Tetris",ANCHO_VENTANA,ALTO_VENTANA,ESCALA_VENTANA);

    mezclar_bag();
    nueva();

    tGBT_Temporizador* timer=gbt_temporizador_crear(0.5);

    while(1){
        gbt_procesar_entrada();
        eGBT_Tecla t=gbt_obtener_tecla_presionada();

        if(t==GBTK_ESCAPE) break;

       if(t == GBTK_a){
    posX--;
    if(colisiona(piezas[pieza_actual], posX, posY)) posX++;
}

if(t == GBTK_d){
    posX++;
    if(colisiona(piezas[pieza_actual], posX, posY)) posX--;
}

if(t == GBTK_s){
    posY++;
    if(colisiona(piezas[pieza_actual], posX, posY)) posY--;
}

if(t == GBTK_w){
    intentar_rotar();
}

        if(gbt_temporizador_consumir(timer)){
            posY++;
            if(colisiona(piezas[pieza_actual],posX,posY)){
                posY--;
                fijar();
                limpiar();
                nueva();
            }
        }

        render();
        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_cerrar();
    return 0;
}
