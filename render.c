/*
 render.c - renderizado gráfico.
 Responsable de la gestión de la ventana, la aplicación de paletas de color,
 y el dibujo de todos los estados del juego (presentación, menú y tablero).
 */

#include <string.h>
#include "render.h"
#include "dibujos.h"
#include "GBT/gbt.h"
#include "Alfabeto.h"
#include "Texto.h"

int escala_dibujo = 1;
int offsetX = 0;
int offsetY = 0;

/* Definición de la paleta de colores para el renderizado del tablero */
static tGBT_ColorRGB paleta[9] = {
    {0,   0,   0},     // 0 negro/transparente
    {0, 255, 255},     // 1 CE
    {255,255,  0},     // 2 AM
    {180,  0,255},     // 3 VI
    {0, 255,  0},      // 4 VE
    {255,  0,  0},     // 5 RO
    {0,   0,255},      // 6 AZ
    {255,140,  0},     // 7 NA
    {0,0,0}            // 8 reservado
};

/* Matrices que definen visualmente las piezas del juego */
static const uint8_t bloque[7][8][8] = {
    {{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE},{CE,CE,CE,CE,CE,CE,CE,CE}},
    {{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM},{AM,AM,AM,AM,AM,AM,AM,AM}},
    {{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI},{VI,VI,VI,VI,VI,VI,VI,VI}},
    {{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE},{VE,VE,VE,VE,VE,VE,VE,VE}},
    {{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO},{RO,RO,RO,RO,RO,RO,RO,RO}},
    {{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ},{AZ,AZ,AZ,AZ,AZ,AZ,AZ,AZ}},
    {{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA},{NA,NA,NA,NA,NA,NA,NA,NA}}
};

static const uint8_t piezas_orig[CANT_PIEZAS][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}
};

/* Configura la paleta de colores activa en el motor gráfico */
void aplicar_paleta(int tipo_paleta) {
    if (tipo_paleta == 0) {
        gbt_aplicar_paleta(paleta, 9, GBT_FORMATO_888);
    } else {
        gbt_aplicar_paleta(NULL, 0, GBT_FORMATO_888);
    }
}

/* Calcula la resolución de la ventana según el estándar CGA o VGA */
void obtener_resolucion_logica(int resolucion, int* ancho, int* alto) {
    if (resolucion == RES_CGA) {
        *ancho = 320;
        *alto = 200;
    } else {
        *ancho = 640;
        *alto = 480;
    }
}

/* Inicializa o recrea la ventana según la configuración actual */
void recrear_ventana(Configuracion* config) {
    int ancho_logico;
    int alto_logico;
    obtener_resolucion_logica(config->resolucion, &ancho_logico, &alto_logico);

    escala_dibujo = (config->resolucion == RES_CGA) ? 1 : 2;

    int ancho_tablero = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
    int alto_tablero  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;
    int ancho_total = ancho_tablero + 80;

    offsetX = (ancho_logico - ancho_total) / 2;
    offsetY = (alto_logico - alto_tablero) / 2;

    gbt_destruir_ventana();
    gbt_crear_ventana("Tetris", ancho_logico, alto_logico, config->escala);
}

/* Dibuja un fondo con patrón de colores */
static void dibujarFondo(int ancho, int alto, int escala) {
    uint8_t colores[] = {9, 11, 12, 13, 14, 10};
    int filas = alto / (8 * escala);
    int cols  = ancho / (8 * escala);
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < cols; c++) {
            for (int py = 0; py < 8 * escala; py++) {
                for (int px = 0; px < 8 * escala; px++) {
                    gbt_dibujar_pixel(c * 8 * escala + px, f * 8 * escala + py, colores[(f + c) % 6]);
                }
            }
        }
    }
}

/* Dibuja un recuadro central para contener texto */
static void dibujarRecuadro(int ancho, int alto, int escala) {
    int margen = 24 * escala;
    int recW = ancho - margen * 2;
    int recH = alto - margen * 2;
    for (int py = 0; py < recH; py++) {
        for (int px = 0; px < recW; px++) {
            gbt_dibujar_pixel(margen + px, margen + py, 0);
        }
    }
}

/* Función principal de renderizado: controla el dibujo basado en el estado del juego */
void render_pantalla(t_tetris* juego, Configuracion* config, int opcion_menu, const Alfabeto* alf, int ancho_logico, int alto_logico) {
    gbt_borrar_backbuffer(0);

    //Colores

    int color_uno;
    int color_dos;

    if(config->paleta==1){
        //MODO VGA
        color_uno=RO;
        color_dos=VE;
    } else {
        //MODO RGB
        color_uno=VE;
        color_dos=AM;
    }

    int viejo_offsetX = offsetX;
    int viejo_offsetY = offsetY;
    int celdas = ancho_logico / (PIXELES_X_LADO * escala_dibujo);
    int filas = alto_logico / (PIXELES_X_LADO * escala_dibujo);

    if (juego->estado == ESTADO_PRESENTACION) {
        offsetX = 0; offsetY = 0;
        dibujarFondo(ancho_logico, alto_logico, escala_dibujo);
        dibujarRecuadro(ancho_logico, alto_logico, escala_dibujo);
        textoDibujar(alf, "TETRIS PIXEL", (celdas - 12) / 2, filas / 3);
        textoDibujar(alf, "UNLAM", (celdas - 5) / 2, (filas / 3) + 2);
        if (juego->mostrar_press) {
            textoDibujar(alf, "PRESS START", (celdas - 11) / 2, filas * 2 / 3);
        }
        offsetX = viejo_offsetX; offsetY = viejo_offsetY;
        return;
    }

    if (juego->estado == ESTADO_INGRESO_NOMBRE) {
        offsetX = 0; offsetY = 0;
        dibujarFondo(ancho_logico, alto_logico, escala_dibujo);
        textoDibujar(alf, "USERNAME", (celdas - 8) / 2, filas / 3);
        int colNombre = (celdas - juego->nombre_len) / 2;
        if (juego->nombre_len > 0) {
            textoDibujar(alf, juego->nombre_jugador, colNombre, filas / 2);
        }
        if (juego->mostrar_cursor) {
            int cx = (colNombre + juego->nombre_len) * (PIXELES_X_LADO * escala_dibujo);
            int cy = (filas / 2) * (PIXELES_X_LADO * escala_dibujo);
            for (int py = 1 * escala_dibujo; py < 7 * escala_dibujo; py++) {
                for (int px = 1 * escala_dibujo; px < 6 * escala_dibujo; px++) {
                    gbt_dibujar_pixel(cx + px, cy + py, 14);
                }
            }
        }
        textoDibujar(alf, "PRESS ENTER", (celdas - 11) / 2, filas * 3 / 4);
        offsetX = viejo_offsetX; offsetY = viejo_offsetY;
        return;
    }

    if (juego->estado == ESTADO_MENU) {
        offsetX = 0; offsetY = 0;
        dibujarFondo(ancho_logico, alto_logico, escala_dibujo);
        dibujarRecuadro(ancho_logico, alto_logico, escala_dibujo);
        textoDibujar(alf, "CONFIG", (celdas - 6) / 2, filas / 4);
        int filaStart = filas / 4 + 3;
        textoDibujar(alf, "PALETA", (celdas - 16) / 2, filaStart);
        textoDibujar(alf, (config->paleta == 0) ? "RGB" : "VGA", (celdas - 16) / 2 + 12, filaStart);
        textoDibujar(alf, "RESOLUCION", (celdas - 16) / 2, filaStart + 2);
        textoDibujar(alf, (config->resolucion == RES_CGA) ? "CGA" : "VGA", (celdas - 16) / 2 + 12, filaStart + 2);
        textoDibujar(alf, "ESCALA", (celdas - 16) / 2, filaStart + 4);
        textoDibujar(alf, (config->escala == 1) ? "STD" : "AMP", (celdas - 16) / 2 + 12, filaStart + 4);
        textoDibujar(alf, "VELOCIDAD", (celdas - 16) / 2, filaStart + 6);
        textoDibujar(alf, (config->velocidad == 1.0f) ? "LENTA" : (config->velocidad == 0.5f) ? "MEDIA" : "RAPIDA", (celdas - 16) / 2 + 12, filaStart + 6);
        textoDibujar(alf, "JUGAR", (celdas - 16) / 2, filaStart + 8);
        int cursorX = ((celdas - 16) / 2 - 2) * (PIXELES_X_LADO * escala_dibujo);
        int cursorY = (filaStart + opcion_menu * 2) * (PIXELES_X_LADO * escala_dibujo) + (2 * escala_dibujo);
        for (int py = 0; py < 4 * escala_dibujo; py++) {
            for (int px = 0; px < 4 * escala_dibujo; px++) {
                gbt_dibujar_pixel(cursorX + px, cursorY + py, 5);
            }
        }
        offsetX = viejo_offsetX; offsetY = viejo_offsetY;
        return;
    }

    //Estados de Juego Activos (JUGANDO, PAUSA, GAMEOVER) - Interfaz 3 paneles
    // Cálculo dinámico de coordenadas base escaladas para que se centre en CGA y VGA
    int tableroAnchoPx = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo; // 80px en CGA
    int tableroAltoPx  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;  // 160px en CGA

    // El tablero real inicia desplazado hacia abajo para darle espacio a "LINES" arriba
    int tableroPxX1 = offsetX;
    int tableroPxY1 = offsetY + (12 * escala_dibujo);
    int tableroPxX2 = tableroPxX1 + tableroAnchoPx - 1;
    int tableroPxY2 = tableroPxY1 + tableroAltoPx - 1;

    // --- PANEL CENTRAL: LINES + TABLERO ---
    int boxCentroX1 = tableroPxX1 - (4 * escala_dibujo);
    int boxCentroX2 = tableroPxX2 + (4 * escala_dibujo);
    int boxCentroY1 = offsetY;
    int boxCentroY2 = tableroPxY2 + (4 * escala_dibujo);

    // Dibujar marco exterior central
    for (int x = boxCentroX1; x <= boxCentroX2; x++) {
        gbt_dibujar_pixel(x, boxCentroY1, color_uno);
        gbt_dibujar_pixel(x, boxCentroY2, color_uno);
    }
    for (int y = boxCentroY1; y <= boxCentroY2; y++) {
        gbt_dibujar_pixel(boxCentroX1, y, color_uno);
        gbt_dibujar_pixel(boxCentroX2, y, color_uno);
    }
    // Línea divisoria interna (Separa LINES del tablero)
    for (int x = boxCentroX1; x <= boxCentroX2; x++) {
        gbt_dibujar_pixel(x, tableroPxY1 - (2 * escala_dibujo), color_uno);
    }

    // Textos de LINES
    dibujar_texto("LINES", boxCentroX1 + (6 * escala_dibujo), boxCentroY1 + (4 * escala_dibujo), color_dos);
    dibujar_numero(juego->lineas_totales, boxCentroX1 + (52 * escala_dibujo), boxCentroY1 + (4 * escala_dibujo), color_dos);

    // Dibujar el contenido del tablero lógico
    for (int y = FILAS_OCULTAS; y < ALTO_TABLERO; y++) {
        for (int x = 0; x < ANCHO_TABLERO; x++) {
            if (juego->tablero[y][x]>0) {
                // El dibujo se desplaza dinámicamente usando los bloques del juego
                int px = tableroPxX1 + (x * PIXELES_X_LADO * escala_dibujo);
                int py = tableroPxY1 + ((y - FILAS_OCULTAS) * PIXELES_X_LADO * escala_dibujo);

                // Forzar el dibujo del bloque usando coordenadas de pantalla calculadas
                int backup_offsetX = offsetX; int backup_offsetY = offsetY;
                offsetX = px; offsetY = py;
                dibujar(bloque[juego->tablero[y][x] - 1], 0, 0);
                offsetX = backup_offsetX; offsetY = backup_offsetY;
            }
        }
    }

    // Dibujar la pieza actual controlada por el usuario
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (juego->piezas[juego->pieza_actual][i][j]) {
                int dy = juego->posY + i - FILAS_OCULTAS;
                if (dy >= 0 && dy < ALTO_VISIBLE) {
                    int px = tableroPxX1 + ((juego->posX + j) * PIXELES_X_LADO * escala_dibujo);
                    int py = tableroPxY1 + (dy * PIXELES_X_LADO * escala_dibujo);

                    int backup_offsetX = offsetX; int backup_offsetY = offsetY;
                    offsetX = px; offsetY = py;
                    dibujar(bloque[juego->pieza_actual], 0, 0);
                    offsetX = backup_offsetX; offsetY = backup_offsetY;
                }
            }
        }
    }


    // --- PANEL IZQUIERDO: STATISTICS ---
    int boxIzqX1 = offsetX - (76 * escala_dibujo);
    int boxIzqY1 = offsetY;
    int boxIzqX2 = boxCentroX1 - (4 * escala_dibujo);
    int boxIzqY2 = boxCentroY2;

    // Dibujar marco exterior izquierdo
    for (int x = boxIzqX1; x <= boxIzqX2; x++) {
        gbt_dibujar_pixel(x, boxIzqY1, color_uno);
        gbt_dibujar_pixel(x, boxIzqY2, color_uno);
    }
    for (int y = boxIzqY1; y <= boxIzqY2; y++) {
        gbt_dibujar_pixel(boxIzqX1, y, color_uno);
        gbt_dibujar_pixel(boxIzqX2, y, color_uno);
    }
    // Línea divisoria de STATISTICS
    for (int x = boxIzqX1; x <= boxIzqX2; x++) {
        gbt_dibujar_pixel(x, tableroPxY1 - (2 * escala_dibujo), color_uno);
    }
    dibujar_texto("STATISTICS", boxIzqX1 + (4 * escala_dibujo), boxIzqY1 + (4 * escala_dibujo), color_dos);

    // Renderizar las 7 piezas fijas apiladas con sus contadores de 3 ceros
    int startY_piezas = boxIzqY1 + (20 * escala_dibujo);
    for (int p = 0; p < CANT_PIEZAS; p++) {
        int piezaPyY = startY_piezas + (p * 20 * escala_dibujo);

        // Dibujo a bajo nivel de la miniatura de la pieza estática de la lista
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (piezas_orig[p][i][j]) {
                    int blockX = boxIzqX1 + (4 * escala_dibujo) + (j * PIXELES_X_LADO * escala_dibujo);
                    int blockY = piezaPyY + (i * PIXELES_X_LADO * escala_dibujo);

                    int backup_offsetX = offsetX; int backup_offsetY = offsetY;
                    offsetX = blockX; offsetY = blockY;
                    dibujar(bloque[p], 0, 0); // Reutiliza tu lógica de dibujo compactado
                    offsetX = backup_offsetX; offsetY = backup_offsetY;
                }
            }
        }
        // Mostrar recuento de piezas usando el array de la estructura con formato de 3 dígitos (000)
        dibujar_numero_ceros3(juego->cant_piezas_usadas[p], boxIzqX1 + (44 * escala_dibujo), piezaPyY + (6 * escala_dibujo), color_dos);
    }


    // --- PANEL DERECHO: SCORE, NEXT, LEVEL, USERNAME, MAX SCORE ---
    int boxDerX1 = boxCentroX2 + (4 * escala_dibujo);
    int boxDerY1 = offsetY;
    int boxDerX2 = boxDerX1 + (72 * escala_dibujo);
    int boxDerY2 = boxCentroY2;

    // Dibujar marco exterior derecho
    for (int x = boxDerX1; x <= boxDerX2; x++) {
        gbt_dibujar_pixel(x, boxDerY1, color_uno);
        gbt_dibujar_pixel(x, boxDerY2, color_uno);
    }
    for (int y = boxDerY1; y <= boxDerY2; y++) {
        gbt_dibujar_pixel(boxDerX1, y, color_uno);
        gbt_dibujar_pixel(boxDerX2, y, color_uno);
    }

    // Bloque SCORE
    dibujar_texto("SCORE", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (6 * escala_dibujo), color_dos);
    dibujar_numero_ceros7(juego->puntaje, boxDerX1 + (6 * escala_dibujo), boxDerY1 + (16 * escala_dibujo), color_dos);

    // Bloque NEXT PIECE
    dibujar_texto("NEXT", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (42 * escala_dibujo), color_dos);
    int previewX = boxDerX1 + (16 * escala_dibujo);
    int previewY = boxDerY1 + (52 * escala_dibujo);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piezas_orig[juego->siguiente][i][j]) {
                int backup_offsetX = offsetX; int backup_offsetY = offsetY;
                offsetX = previewX + (j * PIXELES_X_LADO * escala_dibujo);
                offsetY = previewY + (i * PIXELES_X_LADO * escala_dibujo);
                dibujar(bloque[juego->siguiente], 0, 0);
                offsetX = backup_offsetX; offsetY = backup_offsetY;
            }
        }
    }

    // Bloque LEVEL
    dibujar_texto("LEVEL", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (96 * escala_dibujo), color_uno);
    dibujar_numero_ceros7(juego->nivel, boxDerX1 + (6 * escala_dibujo), boxDerY1 + (106 * escala_dibujo), color_uno);

    // --- ESTADÍSTICA DE JUEGO POR PREFERENCIA DE NOMBRE ---
    if (juego->nombre_len > 0) {
        // 1. Mostrar el Nombre del Jugador Actual
        dibujar_texto("PLAYER", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (130 * escala_dibujo), color_uno);
        dibujar_texto(juego->nombre_jugador, boxDerX1 + (6 * escala_dibujo), boxDerY1 + (140 * escala_dibujo), color_dos);

        // 2. Buscar dinámicamente el récord histórico
        int max_historico = buscar_record_por_nombre(juego->nombre_jugador);

        // Si es un jugador nuevo o supera su marca previa, el récord se actualiza en vivo mientras gana puntos
        if (juego->puntaje > max_historico) {
            max_historico = juego->puntaje;
        }

        // 3. Mostrar el Récord Máximo asociado
        dibujar_texto("MAX SCORE", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (156 * escala_dibujo), color_uno);
        dibujar_numero_ceros7(max_historico, boxDerX1 + (6 * escala_dibujo), boxDerY1 + (166 * escala_dibujo), color_dos);
    } else {
        // Jugador invitado genérico sin estadísticas complejas
        dibujar_texto("GUEST", boxDerX1 + (6 * escala_dibujo), boxDerY1 + (140 * escala_dibujo), color_dos);
    }

    //OVERLAYS DE ESTADOS INTERNOS (PAUSA Y GAMEOVER)
    if (juego->estado == ESTADO_PAUSA) {
        dibujar_texto("PAUSA", tableroPxX1 + (24 * escala_dibujo), tableroPxY1 + (60 * escala_dibujo), color_dos);
    }

    if (juego->estado == ESTADO_GAMEOVER) {
        // Cuadro de fondo para el cartel de Game Over encima del tablero
        for (int y = tableroPxY1 + (30 * escala_dibujo); y < tableroPxY1 + (95 * escala_dibujo); y++) {
            for (int x = tableroPxX1 + (2 * escala_dibujo); x < tableroPxX2 - (2 * escala_dibujo); x++) {
                gbt_dibujar_pixel(x, y, 5); // Fondo rojo sutil
            }
        }
        const char* txt_game_over = "GAME OVER";
        const char* txt_reiniciar = "REINICIAR ENTER";
        const char* txt_salir     = "SALIR ESC";

        //Cálculo del centro del tablero en el eje X
        int centro_tablero_x = tableroPxX1 + (tableroAnchoPx / 2);
        //Cada letra/espacio en dibujar_texto desplaza exactamente: 4 * escala_dibujo píxeles
        int ancho_letca_px = 4 * escala_dibujo;

        // Calcular posición X para centrar "GAME OVER"
        int x_game_over = centro_tablero_x - ((strlen(txt_game_over) * ancho_letca_px) / 2);
        dibujar_texto(txt_game_over, x_game_over, tableroPxY1 + (40 * escala_dibujo), color_dos);

        // Calcular posición X para centrar "REINICIAR ENTER"
        int x_reiniciar = centro_tablero_x - ((strlen(txt_reiniciar) * ancho_letca_px) / 2);
        dibujar_texto(txt_reiniciar, x_reiniciar, tableroPxY1 + (60 * escala_dibujo), color_uno);

        // Calcular posición X para centrar "SALIR ESC"
        int x_salir = centro_tablero_x - ((strlen(txt_salir) * ancho_letca_px) / 2);
        dibujar_texto(txt_salir, x_salir, tableroPxY1 + (76 * escala_dibujo), color_uno);
    }

}
