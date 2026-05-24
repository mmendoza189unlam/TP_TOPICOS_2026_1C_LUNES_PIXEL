/*
Apellido: Mendoza, Micaela Belen
DNI: 45778189
Usuario: mmendoza189unlam
Entrega: Sí

Apellido: Vega, Villalba Ariel
DNI: 32865933
Usuario: arielvegav13
Entrega: Sí

Apellido: Aguirre, Camila Luciana
DNI: 46183931
Usuario: Cami-2901
Entrega: Sí

*/
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

// Variables globales para configuración, estado del juego y temporizadores
Configuracion config;
t_tetris juego;
tGBT_Temporizador* timer;
tGBT_Temporizador* timer_visual;
int opcion_menu = 0;

int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana);

// Guarda las preferencias del usuario en el archivo de configuración
void guardar_config() {
    FILE* f = fopen(ARCHIVO_CONFIG, "wb");
    if (f) {
        fwrite(&config, sizeof(Configuracion), 1, f);
        fclose(f);
    }
}

// Carga las preferencias del usuario o establece valores por defecto
void cargar_config() {
    FILE* f = fopen(ARCHIVO_CONFIG, "rb");
    if (f) {
        fread(&config, sizeof(Configuracion), 1, f);
        fclose(f);
        if (config.resolucion != RES_CGA && config.resolucion != RES_VGA) {
            config.resolucion = RES_CGA;
        }
        if (config.escala < 1 || config.escala > 6) {
            config.escala = 2;
        }
        if (config.velocidad < 0.1f || config.velocidad > 1.0f) {
            config.velocidad = 0.5f;
        }
        if (config.paleta < 0 || config.paleta > 1) {
            config.paleta = 0;
        }
    } else {
        config.paleta = 0;
        config.resolucion = RES_CGA;
        config.escala = 2;
        config.velocidad = 0.5f;
    }
}

int main(int argc, char* argv[]) {
    int ancho_ventana, alto_ventana, escala_ventana;

    // Validación de parámetros desde la línea de comandos
    if (leerArgumentos(argc, argv, &ancho_ventana, &alto_ventana, &escala_ventana) == ERROR) {
        return ERROR;
    }

    srand(time(0));
    //Verificacion de se haya ejecutado bien
    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }
    cargar_config();

    // Actualización de configuración con parámetros recibidos
    config.escala = escala_ventana;
    config.resolucion = (ancho_ventana == 640) ? RES_VGA : RES_CGA;

    aplicar_paleta(config.paleta);
    recrear_ventana(&config);

    // Inicialización de componentes gráficos
    Alfabeto alf;
    alfabetoCrear(&alf);

    // Configuración del estado inicial del juego
    memset(&juego, 0, sizeof(t_tetris));
    juego.mostrar_press = 1;
    juego.mostrar_cursor = 1;
    juego.estado = ESTADO_PRESENTACION;

    timer = gbt_temporizador_crear(config.velocidad);
    timer_visual = gbt_temporizador_crear(0.5);

    // Bucle principal
    while (1) {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        // Control de parpadeo para elementos visuales
        if (gbt_temporizador_consumir(timer_visual)) {
            juego.mostrar_press = !juego.mostrar_press;
            juego.mostrar_cursor = !juego.mostrar_cursor;
        }

        // --- Lógica de estados ---

        // Teclas globales válidas en estados activos de juego o menús
        if (juego.estado == ESTADO_JUGANDO || juego.estado == ESTADO_PAUSA || juego.estado == ESTADO_GAMEOVER) {
            // Si aprieta N, inicia un NUEVO JUEGO directamente
            if (tecla == GBTK_n) {
                tetris_reiniciar(&juego);
                gbt_temporizador_destruir(timer);
                timer = gbt_temporizador_crear(config.velocidad);
                juego.estado = ESTADO_JUGANDO;
            }
            // Si aprieta M, vuelve al MENÚ de configuración
            if (tecla == GBTK_m) {
                juego.estado = ESTADO_MENU;
            }
        }

        if (juego.estado == ESTADO_PRESENTACION) {
            if (tecla == GBTK_ESCAPE) {
                break;
            }
            if (tecla == GBTK_ENTER) {
                juego.estado = ESTADO_INGRESO_NOMBRE;
            }
        } else {
            if (juego.estado == ESTADO_INGRESO_NOMBRE) {
                if (tecla == GBTK_ESCAPE) {
                    break;
                }
                // Captura de teclas para nombre de usuario
                if (tecla >= GBTK_a && tecla <= GBTK_z && juego.nombre_len < 12) {
                    juego.nombre_jugador[juego.nombre_len] = 'A' + (tecla - GBTK_a);
                    juego.nombre_len++;
                    juego.nombre_jugador[juego.nombre_len] = '\0';
                }
                if (tecla == GBTK_RETROCESO && juego.nombre_len > 0) {
                    juego.nombre_len--;
                    juego.nombre_jugador[juego.nombre_len] = '\0';
                }
                if (tecla == GBTK_ENTER && juego.nombre_len > 0) {
                    juego.estado = ESTADO_MENU;
                }
            } else {
                if (juego.estado == ESTADO_MENU) {
                    // Gestión del menú de configuración
                    if (gbt_tecla_presionada(GBTK_ESCAPE)) {
                        break;
                    }
                    if (gbt_tecla_presionada(GBTK_ABAJO)) {
                        opcion_menu = (opcion_menu + 1) % 5;
                    }
                    if (gbt_tecla_presionada(GBTK_ARRIBA)) {
                        opcion_menu = (opcion_menu - 1 + 5) % 5;
                    }
                    // Modificación de configuración con flecha derecha
                    if (gbt_tecla_presionada(GBTK_DERECHA)) {
                        switch (opcion_menu) {
                            case 0: config.paleta = !config.paleta; aplicar_paleta(config.paleta); break;
                            case 1: config.resolucion = (config.resolucion == RES_CGA) ? RES_VGA : RES_CGA; recrear_ventana(&config); break;
                            case 2: config.escala = (config.escala == 1) ? 2 : 1; recrear_ventana(&config); break;
                            case 3: config.velocidad = (config.velocidad == 1.0f) ? 0.5f : (config.velocidad == 0.5f) ? 0.2f : 1.0f; timer = gbt_temporizador_crear(config.velocidad); break;
                        }
                        guardar_config();
                    }
                    if (gbt_tecla_presionada(GBTK_ENTER) && opcion_menu == 4) {
                        tetris_reiniciar(&juego);
                        // Modifica el temporizador base respetando la velocidad elegida en el menú de opciones
                        gbt_temporizador_destruir(timer);
                        timer = gbt_temporizador_crear(config.velocidad);
                        juego.estado = ESTADO_JUGANDO;
                    }
                } else {
                    if (juego.estado == ESTADO_PAUSA) {
                        if (gbt_tecla_presionada(GBTK_p)) {
                            juego.estado = ESTADO_JUGANDO;
                        }
                    } else {
                        if (juego.estado == ESTADO_GAMEOVER) {
                            if (gbt_tecla_presionada(GBTK_ENTER)) {
                                tetris_reiniciar(&juego);

                                gbt_temporizador_destruir(timer);
                                timer = gbt_temporizador_crear(config.velocidad);
                                juego.estado = ESTADO_JUGANDO;
                            }
                            if (gbt_tecla_presionada(GBTK_ESCAPE)) {
                                break;
                            }
                        } else {
                            if (juego.estado == ESTADO_JUGANDO) {

                                // Lógica del juego en ejecución
                                if (gbt_tecla_presionada(GBTK_p)) juego.estado = ESTADO_PAUSA;
                                if (gbt_tecla_presionada(GBTK_ESCAPE)) break;

                                // Entrada de movimientos lógicos usando la estructura integrada
                                if (gbt_tecla_presionada(GBTK_IZQUIERDA)) tetris_mover_izq(&juego);
                                if (gbt_tecla_presionada(GBTK_DERECHA)) tetris_mover_der(&juego);
                                if (gbt_tecla_presionada(GBTK_ABAJO)) tetris_mover_abajo_manual(&juego);
                                if (gbt_tecla_presionada(GBTK_ARRIBA)) tetris_rotar_der(&juego);
                                if (gbt_tecla_presionada(GBTK_w)) tetris_rotar_izq(&juego);
                                if (gbt_tecla_presionada(GBTK_ESPACIO)) tetris_hard_drop(&juego);

                                //Guarda el nivel antes de procesar la grevedad
                                int nivel_previo = juego.nivel;

                                // Lógica del motor de gravedad y el sistema Lock Delay
                                tetris_procesar_gravedad(&juego, timer);

                                if (juego.nivel != nivel_previo) {
                                    float multiplicador_velocidad = juego.intervalo_actual / 1000.0f;

                                    if (timer) {
                                        gbt_temporizador_destruir(timer);
                                    }
                                    // Se crea el nuevo timer con la velocidad acelerada del nuevo nivel
                                    timer = gbt_temporizador_crear(config.velocidad * multiplicador_velocidad);
                                }

                            }
                        }
                    }
                }
            }
        }

        // Renderizado del estado actual
        int ancho_logico, alto_logico;
        obtener_resolucion_logica(config.resolucion, &ancho_logico, &alto_logico);
        render_pantalla(&juego, &config, opcion_menu, &alf, ancho_logico, alto_logico);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    //Liberacion de memoria
    if (timer) gbt_temporizador_destruir(timer);
    if (timer_visual) gbt_temporizador_destruir(timer_visual);
    if (juego.timer_fijacion) gbt_temporizador_destruir(juego.timer_fijacion);

    //Liberacion de memoria de las filas
    for (int y = 0; y < ALTO_TABLERO; y++) {
        if (juego.tablero[y] != NULL) {
            free(juego.tablero[y]);
        }
    }


    gbt_cerrar();
    return 0;
}

// Función de validación de argumentos de entrada
int leerArgumentos(int argc, char* argv[], int* ancho, int* alto, int* escala_ventana) {
    if (argc != PARAMETROS_TOTAL) {
        printf("ERROR 1: La cantidad de parametros no es la esperada\n");
        return ERROR;
    }
    char resolucion[4];
    char escala[2];
    strcpy(resolucion, argv[1]);
    strcpy(escala, argv[2]);

    if (strlen(resolucion) != 3 || (strcmpi(resolucion, "VGA") != 0 && strcmpi(resolucion, "CGA") != 0)) {
        return ERROR;
    }
    if (!isdigit(escala[0]) || escala[0] < '1' || escala[0] > '4') {
        return ERROR;
    }
    if (strcmpi(resolucion, "VGA") == 0 && escala[0] > '2') {
        return ERROR;
    }

    if (strcmpi(resolucion, "VGA") == 0) {
        *ancho = 640;
        *alto = 480;
    } else {
        *ancho = 320;
        *alto = 200;
    }

    *escala_ventana = escala[0] - '0';
    return EXITO;
}
