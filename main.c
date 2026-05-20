#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include "dibujos.h"
#include "GBT/gbt.h"

#define ANCHO_TABLERO 10
#define ALTO_VISIBLE 20
#define FILAS_OCULTAS 4
#define ALTO_TABLERO (ALTO_VISIBLE + FILAS_OCULTAS)
#define CANT_PIEZAS 7
#define ARCHIVO_CONFIG "config.dat"


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

Configuracion config;

int escala_dibujo = 1;
int offsetX = 0;
int offsetY = 0;

tGBT_Temporizador* timer;

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

    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAMEOVER
};

int estado = ESTADO_MENU;
int opcion_menu = 0;

void obtener_resolucion_logica(int resolucion, int* ancho, int* alto){

    if(resolucion == RES_CGA){
        *ancho = 320;
        *alto = 200;
    }
    else{
        *ancho = 640;
        *alto = 480;
    }
}

void recrear_ventana(){

    int ancho_logico;
    int alto_logico;

    obtener_resolucion_logica(config.resolucion, &ancho_logico, &alto_logico);

    if(config.resolucion == RES_CGA){
    escala_dibujo = 1;
}
else{
    escala_dibujo = 2;
}

int ancho_tablero = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
int alto_tablero  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;

int ancho_panel = 80;

int ancho_total = ancho_tablero + ancho_panel;
int alto_total = alto_tablero;

offsetX = (ancho_logico - ancho_total) / 2;
offsetY = (alto_logico - alto_total) / 2;

    gbt_destruir_ventana();

    gbt_crear_ventana("Tetris", ancho_logico, alto_logico, config.escala);
}

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

void rotar_izquierda(uint8_t out[4][4], uint8_t in[4][4]){

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            out[3-j][i]=in[i][j];
}

void intentar_rotar(){
    uint8_t tmp[4][4];
    rotar(tmp, piezas[pieza_actual]);
    if(!colisiona(tmp,posX,posY))
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                piezas[pieza_actual][i][j]=tmp[i][j];
}

void intentar_rotar_izquierda(){

    uint8_t tmp[4][4];

    rotar_izquierda(tmp, piezas[pieza_actual]);

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

   if(estado == ESTADO_MENU){

    dibujar_texto("CONFIG", 30, 20, AM);

    // opciones
    dibujar_texto("PALETA", 10, 50, VE);
    dibujar_texto("RESOLUCION", 10, 65, VE);
    dibujar_texto("ESCALA", 10, 80, VE);
    dibujar_texto("VELOCIDAD", 10, 95, VE);
    dibujar_texto("JUGAR", 10, 110, VE);

    // valor actual de paleta
    if(config.paleta == 0)
        dibujar_texto("RGB", 60, 50, AM);
    else
        dibujar_texto("VGA", 60, 50, AM);

    if(config.resolucion == RES_CGA)
        dibujar_texto("CGA", 60, 65, AM);
    else
        dibujar_texto("VGA", 60, 65, AM);

    if(config.escala == 1)
        dibujar_texto("STD", 60, 80, AM);
    else
        dibujar_texto("AMP", 60, 80, AM);


    if(config.velocidad == 1.0f)
        dibujar_texto("LENTA", 60, 95, AM);

        else if(config.velocidad == 0.5f)
            dibujar_texto("MEDIA", 60, 95, AM);

        else
            dibujar_texto("RAPIDA", 60, 95, AM);

    // cursor rojo
    gbt_dibujar_pixel(2, 52 + opcion_menu*15, RO);
}

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
    int panelX = offsetX + ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo + 10;
    int panelY = offsetY;

    dibujar_texto("SCORE", panelX, panelY + 2, AM);
    dibujar_numero(puntaje, panelX, panelY + 10, AM);
    dibujar_texto("NEXT", panelX, panelY + 30, AM);

// Dibujar preview de siguiente pieza
    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
        if(piezas_orig[siguiente][i][j]){

// posición fija en panel derecho
        dibujar(bloque[siguiente], 11 + j, 6 + i);
        }
    int baseX = offsetX;
    int baseY = offsetY;

    int ancho_px = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
    int alto_px  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;

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

   for(int y = offsetY + 55; y < offsetY + 75; y++){
    for(int x = offsetX + 15; x < offsetX + 105; x++){
        gbt_dibujar_pixel(x, y, AZ);
    }
}

dibujar_texto("PAUSA", offsetX + 40, offsetY + 62, AM);
}

if(estado == ESTADO_GAMEOVER){

    for(int y = offsetY + 45; y < offsetY + 95; y++){
    for(int x = offsetX + 5; x < offsetX + 115; x++){
        gbt_dibujar_pixel(x, y, RO);
        }
    }
    dibujar_texto("GAME OVER", offsetX + 40, offsetY + 50, AM);

    dibujar_texto("REINTENTAR   ENTER", offsetX + 10, offsetY + 68, VE);
    dibujar_texto("SALIR   ESCAPE", offsetX + 28, offsetY + 88, VE);
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

void guardar_config(){

    FILE* f = fopen(ARCHIVO_CONFIG, "wb");

    if(f){
        fwrite(&config, sizeof(Configuracion), 1, f);
        fclose(f);
    }
}

void cargar_config(){

    FILE* f = fopen(ARCHIVO_CONFIG, "rb");

    if(f){

        fread(&config, sizeof(Configuracion), 1, f);
        fclose(f);

        if(config.resolucion != RES_CGA && config.resolucion != RES_VGA)
        config.resolucion = RES_CGA;

        if(config.escala < 1 || config.escala > 6)
        config.escala = 2;

        if(config.velocidad < 0.1f || config.velocidad > 1.0f)
        config.velocidad = 0.5f;

        if(config.paleta < 0 || config.paleta > 1)
        config.paleta = 0;

    }else{

        // valores por defecto

        config.paleta = 0;
        config.resolucion = RES_CGA;
        config.escala = 2;
        config.velocidad = 0.5f;
    }
}


int main(int argc, char* argv[]){

    srand(time(0));
    gbt_iniciar();
    cargar_config();

        if(argc >= 2){
            if(strcmp(argv[1], "CGA") == 0)
            config.resolucion = RES_CGA;
            else if(strcmp(argv[1], "VGA") == 0)
            config.resolucion = RES_VGA;
}

        if(argc >= 3){
            config.escala = atoi(argv[2]);
            }
        if(config.paleta == 0){

            gbt_aplicar_paleta(paleta, 9, GBT_FORMATO_888);
}
else{

        gbt_aplicar_paleta(NULL, 0, GBT_FORMATO_888);
}


    int ancho_logico;
int alto_logico;

obtener_resolucion_logica(config.resolucion, &ancho_logico, &alto_logico);

if(config.resolucion == RES_CGA){
    escala_dibujo = 1;
}
else{
    escala_dibujo = 2;
}

int ancho_tablero = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
int alto_tablero  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;

int ancho_panel = 80;

int ancho_total = ancho_tablero + ancho_panel;
int alto_total = alto_tablero;

offsetX = (ancho_logico - ancho_total) / 2;
offsetY = (alto_logico - alto_total) / 2;

gbt_crear_ventana("Tetris", ancho_logico, alto_logico, config.escala);

    memcpy(piezas, piezas_orig, sizeof(piezas_orig));  // ← primero
    mezclar_bag();
    siguiente = siguiente_pieza(); // preload
    nueva();
    timer = gbt_temporizador_crear(config.velocidad);

while(1){

    gbt_procesar_entrada();

    // PAUSA
    if(gbt_tecla_presionada(GBTK_p)){

        if(estado == ESTADO_JUGANDO)
            estado = ESTADO_PAUSA;

        else if(estado == ESTADO_PAUSA)
            estado = ESTADO_JUGANDO;
    }

    // MOSTRAR MENU

    if(gbt_tecla_presionada(GBTK_m)){

    estado = ESTADO_MENU;
}

    //REINICIAR PARTIDA

    if(gbt_tecla_presionada(GBTK_n)){

    reiniciar_juego();

    estado = ESTADO_JUGANDO;
}

if(estado == ESTADO_MENU){

    if(gbt_tecla_presionada(GBTK_ABAJO)){
        opcion_menu++;

        if(opcion_menu > 4)
            opcion_menu = 0;
    }

    if(gbt_tecla_presionada(GBTK_ARRIBA)){
        opcion_menu--;

        if(opcion_menu < 0)
            opcion_menu = 4;
    }

    // CAMBIAR OPCIONES
    if(gbt_tecla_presionada(GBTK_DERECHA)){

        switch(opcion_menu){

    case 0: // PALETA

        config.paleta++;

        if(config.paleta > 1)
            config.paleta = 0;

        if(config.paleta == 0){
            gbt_aplicar_paleta(paleta, 9, GBT_FORMATO_888);
        }else{
            gbt_aplicar_paleta(NULL, 0, GBT_FORMATO_888);
        }

        break;

    case 1: // RESOLUCION

        if(config.resolucion == RES_CGA)
            config.resolucion = RES_VGA;
        else
            config.resolucion = RES_CGA;

        recrear_ventana();

        break;

    case 2: // ESCALA

        config.escala++;

        if(config.escala > 2)
            config.escala = 1;

        recrear_ventana();

        break;

    case 3: // VELOCIDAD

        if(config.velocidad == 1.0f){

            config.velocidad = 0.5f;
        }
            else if(config.velocidad == 0.5f){

                config.velocidad = 0.2f;
            }
            else{

                config.velocidad = 1.0f;
                }

        timer = gbt_temporizador_crear(config.velocidad);

    break;
}

        guardar_config();
    }

    // INICIAR JUEGO
    if(gbt_tecla_presionada(GBTK_ENTER)){

        if(opcion_menu == 4){

            estado = ESTADO_JUGANDO;
        }
    }
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

        //ROTAR A IZQUIERDA
        if(gbt_tecla_presionada(GBTK_w)){
            intentar_rotar_izquierda();
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
