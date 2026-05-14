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

tGBT_ColorRGB paleta[9] = {

    {0,   0,   0},     // 0 negro/transparente

    {0, 255, 255},     // 1 CE
    {255,255,  0},     // 2 AM
    {180,  0,255},     // 3 VI
    {0, 255,  0},      // 4 VE
    {255,  0,  0},     // 5 RO
    {0,   0,255},      // 6 AZ
    {255,140,  0},     // 7 NA

    {0,0,0}            // 8 reservado transparente GBT
};

/// BLOQUES VISUALES
const uint8_t bloque[7][8][8] = {

{{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE}}, // I

{{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM}}, // O

{{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI}}, // T

{{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE}}, // S

{{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO}}, // Z

{{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ}}, // J

{{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA}}  // L

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

enum {
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
};

int estado = ESTADO_JUGANDO;

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
    estado = ESTADO_GAMEOVER;
}
}

void reiniciar_juego();

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
                gbt_dibujar_pixel(84 + k*(ANCHO_DIGITO+1) + lx, 2 + ly, AM);  // 2 + ly

// Panel de puntaje (a la derecha, desde x=82)
// Etiqueta "SC" en píxeles (simplificado, podés expandirlo)
    dibujar_numero(puntaje, 84, 10, AM); // color amarillo

// Dibujar "NEXT"
    for(int k = 0; k < 4; k++)
    for(int ly = 0; ly < 5; ly++)
        for(int lx = 0; lx < 3; lx++)
            if(letras_next[k][ly][lx])
                gbt_dibujar_pixel(84 + k*(ANCHO_DIGITO+1) + lx, 30 + ly, AM);

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
    gbt_dibujar_pixel(baseX + x, baseY, VE); // arriba
    gbt_dibujar_pixel(baseX + x, baseY + alto_px - 1, VE); // abajo
}

// Línea izquierda y derecha
for(int y = 0; y < alto_px; y++){
    gbt_dibujar_pixel(baseX, baseY + y, VE); // izquierda
    gbt_dibujar_pixel(baseX + ancho_px - 1, baseY + y, VE); // derecha
}

if(estado == ESTADO_PAUSA){

    for(int y=55;y<75;y++)
        for(int x=15;x<105;x++)
            gbt_dibujar_pixel(x,y,AZ);

    dibujar_texto("PAUSA", 40, 62, AM);
}

if(estado == ESTADO_GAMEOVER){

    for(int y=45;y<95;y++)
        for(int x=5;x<115;x++)
            gbt_dibujar_pixel(x,y,RO);

    dibujar_texto("GAME OVER", 25, 55, AM);

    dibujar_texto("ENTER", 38, 70, VE);

    dibujar_texto("REINTENTAR", 20, 80, VE);
}

}

void reiniciar_juego(){

    memset(tablero, 0, sizeof(tablero));

    puntaje = 0;
    game_over = 0;
    estado = ESTADO_JUGANDO;

    memcpy(piezas, piezas_orig, sizeof(piezas_orig));

    mezclar_bag();

    siguiente = siguiente_pieza();

    nueva();
}

int main(){

    srand(time(0));
    gbt_iniciar();
    gbt_aplicar_paleta(paleta, 9, GBT_FORMATO_888);
    gbt_crear_ventana("Tetris",ANCHO_VENTANA,ALTO_VENTANA,ESCALA_VENTANA);

    memcpy(piezas, piezas_orig, sizeof(piezas_orig));  // ← primero
    mezclar_bag();
    siguiente = siguiente_pieza(); // preload
    nueva();
    tGBT_Temporizador* timer = gbt_temporizador_crear(0.5);

while(1){

    gbt_procesar_entrada();

    // PAUSA
    if(gbt_tecla_presionada(GBTK_p)){

        if(estado == ESTADO_JUGANDO)
            estado = ESTADO_PAUSA;

        else if(estado == ESTADO_PAUSA)
            estado = ESTADO_JUGANDO;
    }

    if(estado == ESTADO_GAMEOVER){

    if(gbt_tecla_presionada(GBTK_ENTER)){
        reiniciar_juego();
    }
}

    // SALIR
    if(gbt_tecla_presionada(GBTK_ESCAPE)){
        break;
    }

    // =========================
    // SOLO JUEGA SI ESTÁ ACTIVO
    // =========================
    if(estado == ESTADO_JUGANDO){

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

        // ABAJO
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

        // HARD DROP
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

        // CAÍDA AUTOMÁTICA
        if(gbt_temporizador_consumir(timer)){
            posY++;
            if(colisiona(piezas[pieza_actual],posX,posY)){
                posY--;
                fijar();
                limpiar();
                nueva();
            }
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
