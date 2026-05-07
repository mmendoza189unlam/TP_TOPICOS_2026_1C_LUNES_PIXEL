#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include "dibujos.h"
#include "GBT/gbt.h"

#define ANCHO_VENTANA 120
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

const uint8_t piezas_orig[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}
};

// Copia mutable que se modifica al rotar
uint8_t piezas[CANT_PIEZAS][4][4];

int game_over = 0;
int puntaje = 0;
int pieza_actual, posX, posY;
int siguiente;
int bag[7], bag_index=0;

void mezclar_bag(){
    for(int i=0;i<7;i++) bag[i]=i;
    for(int i=6;i>0;i--){      // <-- empieza desde el final
        int j=rand()%(i+1);    // <-- rango se achica cada iteración
        int t=bag[i];bag[i]=bag[j];bag[j]=t;
    }
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
    int filas = 0;
    for(int y=0;y<ALTO_TABLERO;y++){
        int llena=1;
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(!tablero[y][x]) llena=0;
        if(llena){
            for(int yy=y;yy>0;yy--)
                for(int x=0;x<ANCHO_TABLERO;x++)
                    tablero[yy][x]=tablero[yy-1][x];
            y--;
            filas++;
        }
    }
    int tabla[] = {0, 100, 300, 500, 800};
    if(filas > 0) puntaje += tabla[filas];
}



void nueva(){
    pieza_actual = siguiente;
    siguiente = siguiente_pieza();

    memcpy(piezas[pieza_actual], piezas_orig[pieza_actual], sizeof(piezas[0]));

    posX = 3;
    posY = 0;

    if(colisiona(piezas[pieza_actual], posX, posY)){
        game_over = 1;
    }
}

void render(){
    gbt_borrar_backbuffer(0);

    // Tablero y pieza actual (igual que antes)
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

// Dibujar "SCORE"
    for(int k = 0; k < 5; k++)
    for(int ly = 0; ly < 5; ly++)   // ly en lugar de y
        for(int lx = 0; lx < 3; lx++)  // lx en lugar de x
            if(letras_score[k][ly][lx])
                gbt_dibujar_pixel(84 + k*(ANCHO_DIGITO+1) + lx, 2 + ly, A);  // 2 + ly

// Puntaje debajo del texto (solo una vez)
    dibujar_numero(puntaje, 84, 10, A);

// Panel de puntaje (a la derecha, desde x=82)
// Etiqueta "SC" en píxeles (simplificado, podés expandirlo)
    dibujar_numero(puntaje, 84, 10, A); // color amarillo

// Dibujar "NEXT"
    for(int k = 0; k < 4; k++)
    for(int ly = 0; ly < 5; ly++)
        for(int lx = 0; lx < 3; lx++)
            if(letras_next[k][ly][lx])
                gbt_dibujar_pixel(84 + k*(ANCHO_DIGITO+1) + lx, 30 + ly, A);

// Dibujar preview de siguiente pieza
    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
        if(piezas_orig[siguiente][i][j]){
// posición fija en panel derecho
        dibujar(bloque[siguiente], 11 + j, 6 + i);
        }
    int baseX = 0;
    int baseY = 0;

    int ancho_px = ANCHO_TABLERO * PIXELES_X_LADO;
    int alto_px  = ALTO_VISIBLE * PIXELES_X_LADO;

// Línea superior e inferior
for(int x = 0; x < ancho_px; x++){
    gbt_dibujar_pixel(baseX + x, baseY, V); // arriba
    gbt_dibujar_pixel(baseX + x, baseY + alto_px - 1, V); // abajo
}

// Línea izquierda y derecha
for(int y = 0; y < alto_px; y++){
    gbt_dibujar_pixel(baseX, baseY + y, V); // izquierda
    gbt_dibujar_pixel(baseX + ancho_px - 1, baseY + y, V); // derecha
}
}

int main(){

    srand(time(0));
    gbt_iniciar();
    gbt_crear_ventana("Tetris",ANCHO_VENTANA,ALTO_VENTANA,ESCALA_VENTANA);

    memcpy(piezas, piezas_orig, sizeof(piezas_orig));  // ← primero
    mezclar_bag();
    siguiente = siguiente_pieza(); // preload
    nueva();
    tGBT_Temporizador* timer = gbt_temporizador_crear(0.5);
    int delay_mov = 0;

while(!game_over){

gbt_procesar_entrada();

// IZQUIERDA
if(gbt_tecla_presionada(GBTK_IZQUIERDA)){
    posX--;
    if(colisiona(piezas[pieza_actual], posX, posY)) posX++;
}

// DERECHA
if(gbt_tecla_presionada(GBTK_DERECHA)){
    posX++;
    if(colisiona(piezas[pieza_actual], posX, posY)) posX--;
}

// ABAJO (1 paso por toque)
if(gbt_tecla_presionada(GBTK_ABAJO)){
    posY++;
    if(colisiona(piezas[pieza_actual], posX, posY)){
        posY--;
    } else {
        puntaje += 1;
    }
}

// ROTAR
if(gbt_tecla_presionada(GBTK_ARRIBA)){
    intentar_rotar();
}

// HARD DROP (ESPACIO)
if(gbt_tecla_presionada(GBTK_ESPACIO)){
    while(!colisiona(piezas[pieza_actual], posX, posY)){
        posY++;
        puntaje += 2;
    }
    posY--;
    fijar();
    limpiar();
    nueva();
}

// SALIR
if(gbt_tecla_presionada(GBTK_ESCAPE)){
    break;
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

    printf("GAME OVER\n");
    gbt_cerrar(); // ahora sí se llama correctamente
    return 0;
}
