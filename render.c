/*
 render.c - renderizado gráfico.
 Responsable de la gestión de la ventana, la aplicación de paletas de color,
 y el dibujo de todos los estados del juego (presentación, menú y tablero).
 */

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

    // Dibujo del tablero de juego, piezas activas y panel de puntaje
    for (int y = FILAS_OCULTAS; y < ALTO_TABLERO; y++) {
        for (int x = 0; x < ANCHO_TABLERO; x++) {
            if (juego->tablero[y][x]) {
                dibujar(bloque[juego->tablero[y][x] - 1], x, y - FILAS_OCULTAS);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (juego->piezas[juego->pieza_actual][i][j]) {
                int dy = juego->posY + i - FILAS_OCULTAS;
                if (dy >= 0) {
                    dibujar(bloque[juego->pieza_actual], juego->posX + j, dy);
                }
            }
        }
    }
    int panelX = offsetX + ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo + 10;
    dibujar_texto("SCORE", panelX, offsetY + 2, AM);
    dibujar_numero(juego->puntaje, panelX, offsetY + 10, AM);
    dibujar_texto("NEXT", panelX, offsetY + 30, AM);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piezas_orig[juego->siguiente][i][j]) {
                dibujar(bloque[juego->siguiente], 11 + j, 6 + i);
            }
        }
    }
    // Bordes del tablero
    int ancho_px = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
    int alto_px = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;
    for (int x = 0; x < ancho_px; x++) {
        gbt_dibujar_pixel(offsetX + x, offsetY, VE);
        gbt_dibujar_pixel(offsetX + x, offsetY + alto_px - 1, VE);
    }
    for (int y = 0; y < alto_px; y++) {
        gbt_dibujar_pixel(offsetX, offsetY + y, VE);
        gbt_dibujar_pixel(offsetX + ancho_px - 1, offsetY + y, VE);
    }
    if (juego->estado == ESTADO_PAUSA) {
        dibujar_texto("PAUSA", offsetX + 40, offsetY + 62, AM);
    }
    if (juego->estado == ESTADO_GAMEOVER) {
        dibujar_texto("GAME OVER", offsetX + 40, offsetY + 50, AM);
    }
}
