#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "GBT/gbt.h"
#include "tetris.h"
#include "render.h"
#include "alfabeto.h"
#include "texto.h"

#define PARAMETROS_TOTAL 3
#define EXITO 0
#define ERROR 1

Configuracion config;
t_tetris juego;
tGBT_Temporizador* timer;
tGBT_Temporizador* timer_visual;
int opcion_menu = 0;

// Firma de tu función
int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana);

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

        if(config.resolucion != RES_CGA && config.resolucion != RES_VGA) config.resolucion = RES_CGA;
        if(config.escala < 1 || config.escala > 6) config.escala = 2;
        if(config.velocidad < 0.1f || config.velocidad > 1.0f) config.velocidad = 0.5f;
        if(config.paleta < 0 || config.paleta > 1) config.paleta = 0;
    } else {
        config.paleta = 0;
        config.resolucion = RES_CGA;
        config.escala = 2;
        config.velocidad = 0.5f;
    }
}

int main(int argc, char* argv[]){
    int ancho_ventana, alto_ventana, escala_ventana;

    // 1. Usamos tu validación impecable de argumentos ANTES de iniciar todo
    if(leerArgumentos(argc, argv, &ancho_ventana, &alto_ventana, &escala_ventana) == ERROR){
        return ERROR;
    }

    srand(time(0));
    gbt_iniciar();
    cargar_config();

    // 2. Pisamos la configuración cargada con lo que el usuario mandó por consola (Requisito del TP)
    config.escala = escala_ventana;
    if(ancho_ventana == 640) config.resolucion = RES_VGA;
    else config.resolucion = RES_CGA;

    aplicar_paleta(config.paleta);
    recrear_ventana(&config);

    // 3. Inicializamos tu Alfabeto
    Alfabeto alf;
    alfabetoCrear(&alf);

    // 4. Preparamos el estado inicial del jugador
    memset(&juego, 0, sizeof(t_tetris));
    juego.mostrar_press = 1;
    juego.mostrar_cursor = 1;
    juego.estado = ESTADO_PRESENTACION; // ¡Arranca directamente en tu pantalla!

    timer = gbt_temporizador_crear(config.velocidad);
    timer_visual = gbt_temporizador_crear(0.5);

    // 5. MÁQUINA DE ESTADOS PRINCIPAL
    while(1){
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if(gbt_temporizador_consumir(timer_visual)){
            juego.mostrar_press = !juego.mostrar_press;
            juego.mostrar_cursor = !juego.mostrar_cursor;
        }

        if(juego.estado == ESTADO_PRESENTACION){
            if(tecla == GBTK_ESCAPE) break;
            if(tecla == GBTK_ENTER){
                juego.estado = ESTADO_INGRESO_NOMBRE;
            }
        }
        else if(juego.estado == ESTADO_INGRESO_NOMBRE){
            if(tecla == GBTK_ESCAPE) break;

            if(tecla >= GBTK_a && tecla <= GBTK_z && juego.nombre_len < 12){
                juego.nombre_jugador[juego.nombre_len] = 'A' + (tecla - GBTK_a);
                juego.nombre_len++;
                juego.nombre_jugador[juego.nombre_len] = '\0';
            }

            if(tecla == GBTK_RETROCESO && juego.nombre_len > 0){
                juego.nombre_len--;
                juego.nombre_jugador[juego.nombre_len] = '\0';
            }

            if(tecla == GBTK_ENTER && juego.nombre_len > 0){
                juego.estado = ESTADO_MENU;
            }
        }
        else if(juego.estado == ESTADO_MENU){
            if(gbt_tecla_presionada(GBTK_ESCAPE)) break;

            if(gbt_tecla_presionada(GBTK_ABAJO)){
                opcion_menu++;
                if(opcion_menu > 4) opcion_menu = 0;
            }
            if(gbt_tecla_presionada(GBTK_ARRIBA)){
                opcion_menu--;
                if(opcion_menu < 0) opcion_menu = 4;
            }

            if(gbt_tecla_presionada(GBTK_DERECHA)){
                switch(opcion_menu){
                    case 0:
                        config.paleta++;
                        if(config.paleta > 1) config.paleta = 0;
                        aplicar_paleta(config.paleta);
                        break;
                    case 1:
                        if(config.resolucion == RES_CGA) config.resolucion = RES_VGA;
                        else config.resolucion = RES_CGA;
                        recrear_ventana(&config);
                        break;
                    case 2:
                        config.escala++;
                        if(config.escala > 2) config.escala = 1;
                        recrear_ventana(&config);
                        break;
                    case 3:
                        if(config.velocidad == 1.0f) config.velocidad = 0.5f;
                        else if(config.velocidad == 0.5f) config.velocidad = 0.2f;
                        else config.velocidad = 1.0f;
                        timer = gbt_temporizador_crear(config.velocidad);
                        break;
                }
                guardar_config();
            }

            if(gbt_tecla_presionada(GBTK_ENTER)){
                if(opcion_menu == 4){
                    tetris_reiniciar(&juego);
                    juego.estado = ESTADO_JUGANDO;
                }
            }
        }
        else if(juego.estado == ESTADO_PAUSA){
            if(gbt_tecla_presionada(GBTK_p)) juego.estado = ESTADO_JUGANDO;
        }
        else if(juego.estado == ESTADO_GAMEOVER){
            if(gbt_tecla_presionada(GBTK_ENTER)){
                tetris_reiniciar(&juego);
                juego.estado = ESTADO_JUGANDO;
            }
            if(gbt_tecla_presionada(GBTK_ESCAPE)) break;
        }
        else if(juego.estado == ESTADO_JUGANDO){
            if(gbt_tecla_presionada(GBTK_p)) juego.estado = ESTADO_PAUSA;
            if(gbt_tecla_presionada(GBTK_ESCAPE)) break;

            if(gbt_tecla_presionada(GBTK_IZQUIERDA)) tetris_mover_izq(&juego);
            if(gbt_tecla_presionada(GBTK_DERECHA)) tetris_mover_der(&juego);
            if(gbt_tecla_presionada(GBTK_ABAJO)) tetris_mover_abajo(&juego);
            if(gbt_tecla_presionada(GBTK_ARRIBA)) tetris_rotar_der(&juego);
            if(gbt_tecla_presionada(GBTK_w)) tetris_rotar_izq(&juego);
            if(gbt_tecla_presionada(GBTK_ESPACIO)) tetris_hard_drop(&juego);

            if(gbt_temporizador_consumir(timer)){
                tetris_tick(&juego);
            }
        }

        int ancho_logico, alto_logico;
        obtener_resolucion_logica(config.resolucion, &ancho_logico, &alto_logico);
        render_pantalla(&juego, &config, opcion_menu, &alf, ancho_logico, alto_logico);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    printf("GAME OVER\n");
    gbt_cerrar();
    return 0;
}

// 6. TU FUNCION INTACTA
int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana)
{
    if(argc != PARAMETROS_TOTAL)
    {
        printf("ERROR 1: La cantidad de parametros no es la esperada\n");
        printf("Uso: tetris.exe [CGA|VGA] [1-4]\n");
        printf("Ejemplo: tetris.exe CGA 3\n");
        return ERROR;
    }
    char resolucion[4];
    char escala[2];

    strcpy(resolucion, argv[1]);
    strcpy(escala, argv[2]);

    if(strlen(resolucion) != 3)
    {
        printf("ERROR: Resolucion invalida\n");
        printf("Resoluciones validas: CGA, VGA\n");
        return ERROR;
    }

    if(strcmpi(resolucion, "VGA") != 0 && strcmpi(resolucion, "CGA") != 0)
    {
        printf("ERROR: Resolucion invalida\n");
        printf("Resoluciones validas: CGA, VGA\n");
        return ERROR;
    }

    if(!isdigit(escala[0]))
    {
        printf("ERROR: Escala invalida\n");
        printf("La escala debe ser un numero entre 1 y 4\n");
        return ERROR;
    }

    if(escala[0] < '1' || escala[0] > '4')
    {
        printf("ERROR: Escala invalida\n");
        printf("La escala debe ser un numero entre 1 y 4\n");
        return ERROR;
    }

    if(strcmpi(resolucion, "VGA") == 0 && escala[0] > '2')
    {
        printf("ERROR: Para VGA la escala debe ser 1 o 2\n");
        printf("Con escala mayor la ventana excede el tamanio del monitor\n");
        return ERROR;
    }

    if(strcmpi(resolucion, "VGA") == 0 )
    {
        *ancho = 640;
        *alto = 480;
    } else{
        *ancho = 320;
        *alto = 200;
    }

    *escala_ventana = escala[0] - '0';
    return EXITO;
}
