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
//variables --> aceleracion
int contador_fichas = 0;       // Cant de piezas q se han fijado
float intervalo_actual = 1000; //Lo pongo aca por si queres en un futuro agregar uno q muestre la velocidad al usuario

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

//Modifique lo de ROTAR para q se pueda hacer tanto de izq como derecha
void rotarDerecha(uint8_t out[4][4], uint8_t in[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            out[j][3-i]=in[i][j];
}
//Agregue esta para q se pueda hacer de izq
void rotarIzq(uint8_t out[4][4], uint8_t in[4][4]){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            out[3-j][i]=in[i][j];
}

void intentar_rotar(int haciaDerecha){
    uint8_t tmp[4][4];
    //Es una band q si quiero q rote a la derecha es 1, pero si se quiere rotar a la derecha es 0
    if(haciaDerecha){
        rotarDerecha(tmp, piezas[pieza_actual]);
    } else{
        rotarIzq(tmp, piezas[pieza_actual]);
    }

    if(!colisiona(tmp,posX,posY))
        memcpy(piezas[pieza_actual], tmp, sizeof(tmp)); //Aplico la rotacion a la pieza
}

void fijar(){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(piezas[pieza_actual][i][j])
                tablero[posY+i][posX+j]=pieza_actual+1;
}

void limpiar(int nivel){
    int filas = 0;
    for(int y=0;y<ALTO_TABLERO;y++){
        int llena=1;
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(!tablero[y][x]) llena=0;

        if(llena){
            for(int yy=y;yy>0;yy--){
                for(int x=0;x<ANCHO_TABLERO;x++)
                {
                    tablero[yy][x]=tablero[yy-1][x];
                }

            }

            y--;
            filas++;
        }
    }

    int tabla[] = {0, 100, 300, 500, 800};

    if(filas > 0){
        int multiplicador=(nivel+1);
        int bono_velocidad=(nivel)*5; //para aumentar el puntaje segun el nivel
        int ptos_obtenidos=(tabla[filas]*multiplicador)+bono_velocidad;
        puntaje += ptos_obtenidos;

        //Para visualizar los cambios
        printf("[FILAS] Borradas: %d | Multiplicador Nivel: x%d | Bono: +%d | Puntos: +%d\n",filas, multiplicador, bono_velocidad, ptos_obtenidos);
    }
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
    //Tablero
    for(int y=FILAS_OCULTAS;y<ALTO_TABLERO;y++)
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(tablero[y][x])
                dibujar(bloque[tablero[y][x]-1],x,y-FILAS_OCULTAS);
    //Pieza Actual
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(piezas[pieza_actual][i][j]){
                int dy=posY+i-FILAS_OCULTAS;
                if(dy>=0)
                    dibujar(bloque[pieza_actual],posX+j,dy);
            }

//--PANEL DERECHO--

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

//Dibujar "LEVEL"
    for(int k = 0; k < 5; k++)
        for(int ly = 0; ly < 5; ly++)
            for(int lx = 0; lx < 3; lx++)
                if(letras_level[k][ly][lx])
                    gbt_dibujar_pixel(84 + k*(ANCHO_DIGITO+1) + lx, 100 + ly, V);

//Dibujar el numero de level
    int level = contador_fichas / 10;
    dibujar_numero_ceros(level, 84, 108, V);

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

//Aceleracion
void actualizar_dificultad(tGBT_Temporizador** timer, int* cont, float* intervalo) {
    (*cont)++;
    if ((*cont) % 10 == 0) { // El PDF pide cada 10 fichas
        (*intervalo) *= 0.97f;

        gbt_temporizador_destruir(*timer);
        *timer = gbt_temporizador_crear((*intervalo) / 1000.0f);

        printf("Dificultad aumentada! Nuevo intervalo: %.2f s\n", (*intervalo) / 1000.0f); //Te avisa del nuevo intervalo
    }
}

int main(){

    srand(time(0));

    //Le agregue aca la "verificacion" de q se haya ejecutado bien, tanto al iniciar como cuando abre la ventana
    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_crear_ventana("Tetris",ANCHO_VENTANA,ALTO_VENTANA,ESCALA_VENTANA) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    memcpy(piezas, piezas_orig, sizeof(piezas_orig));  // ← primero
    mezclar_bag();
    siguiente = siguiente_pieza(); // preload
    nueva();
    tGBT_Temporizador* timer = gbt_temporizador_crear(0.5);
    int delay_mov = 0;


    while(!game_over){
        gbt_procesar_entrada();

        //variable --> limpiar, es para a mayor velocidad, mayor los puntos obtenidos
        int nivel= contador_fichas/10;

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
                int ptos_bajar= 1+(nivel*5);
                puntaje += ptos_bajar; //Para el aumento de puntaje segun el nivel

                //Para visualizar los cambios
                printf("[SOFT DROP] Puntos: +%d | Total: %d\n", ptos_bajar, puntaje);
            }
        }

        // ROTAR
        //Lo modifique para q se pueda rotar en ambos sentidos (izq y derecha)
        //ROTAR DERECHA
        if(gbt_tecla_presionada(GBTK_ARRIBA)){
            intentar_rotar(1);
        }
        //ROTAR IZQUIERDA
        if (gbt_tecla_presionada(GBTK_z)){ //Puse que se pueda modificar para la izquierda con la tecla Z, pero va  a ser mejor q sea con otra tecla
            intentar_rotar(0);
        }

        // HARD DROP (ESPACIO)
        if(gbt_tecla_presionada(GBTK_ESPACIO)){
            int celdas_caidas=0;
            while(!colisiona(piezas[pieza_actual], posX, posY)){
                posY++;
                celdas_caidas++;
            }
            int ptos_drop=celdas_caidas*(2+nivel);
            puntaje += ptos_drop; //Para el aumento de puntaje segun el nivel
            posY--;

            //Para visualizar el cambio
            printf("[HARD DROP] Celdas: %d | Puntos: +%d\n", celdas_caidas, ptos_drop);

            fijar();
            limpiar(nivel); //modifico para el aumento segun el nivel

            //Agregué esto para que las piezas de espacio también sumen al contador
            actualizar_dificultad(&timer, &contador_fichas, &intervalo_actual);

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
                limpiar(nivel); //modifico para el aumento segun el nivel
                actualizar_dificultad(&timer,&contador_fichas,&intervalo_actual);

                //Para visualizar el cambio
                printf("[PIEZA FIJADA] Total piezas: %d | Nivel actual: %d | Velocidad: %.2f ms\n",contador_fichas, nivel, intervalo_actual);

                nueva();
            }
        }

        render();
        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    printf("GAME OVER\n");

    //Aqui, no estoy muy segura de esto, pero le agregue lo de limpieza de memoria;
    gbt_temporizador_destruir(timer);
    gbt_destruir_ventana();

    gbt_cerrar(); // ahora sí se llama correctamente
    return 0;
}
