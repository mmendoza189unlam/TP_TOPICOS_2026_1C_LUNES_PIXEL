#include "render.h"
#include "dibujos.h"
#include "GBT/gbt.h"
#include "alfabeto.h"
#include "texto.h"

// Variables globales para que dibujos.c las pueda leer
int escala_dibujo = 1;
int offsetX = 0;
int offsetY = 0;

static tGBT_ColorRGB paleta[9] = {
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

void aplicar_paleta(int tipo_paleta){
    if(tipo_paleta == 0) {
        gbt_aplicar_paleta(paleta, 9, GBT_FORMATO_888);
    } else {
        gbt_aplicar_paleta(NULL, 0, GBT_FORMATO_888);
    }
}

void obtener_resolucion_logica(int resolucion, int* ancho, int* alto){
    if(resolucion == RES_CGA){
        *ancho = 320;
        *alto = 200;
    } else {
        *ancho = 640;
        *alto = 480;
    }
}

void recrear_ventana(Configuracion* config){
    int ancho_logico;
    int alto_logico;
    obtener_resolucion_logica(config->resolucion, &ancho_logico, &alto_logico);

    if(config->resolucion == RES_CGA){
        escala_dibujo = 1;
    } else {
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
    gbt_crear_ventana("Tetris", ancho_logico, alto_logico, config->escala);
}

// Ahora el fondo contempla la escala para no quedar chiquito en VGA
static void dibujarFondo(int ancho, int alto, int escala) {
    uint8_t colores[] = {9, 11, 12, 13, 14, 10};
    int filas = alto / (8 * escala);
    int cols  = ancho / (8 * escala);
    for(int f = 0; f < filas; f++) {
        for(int c = 0; c < cols; c++) {
            for(int py = 0; py < 8 * escala; py++) {
                for(int px = 0; px < 8 * escala; px++) {
                    gbt_dibujar_pixel(c * 8 * escala + px, f * 8 * escala + py, colores[(f+c) % 6]);
                }
            }
        }
    }
}

static void dibujarRecuadro(int ancho, int alto, int escala) {
    int margen = 24 * escala;
    int recW = ancho - margen * 2;
    int recH = alto  - margen * 2;
    for(int py = 0; py < recH; py++) {
        for(int px = 0; px < recW; px++) {
            gbt_dibujar_pixel(margen+px, margen+py, 0); // Centro en negro
        }
    }
}

void render_pantalla(t_tetris* juego, Configuracion* config, int opcion_menu, const Alfabeto* alf, int ancho_logico, int alto_logico) {
    gbt_borrar_backbuffer(0);

    // Guardamos los offsets del tablero para no perderlos
    int viejo_offsetX = offsetX;
    int viejo_offsetY = offsetY;

    int celdas = ancho_logico / (PIXELES_X_LADO * escala_dibujo);
    int filas  = alto_logico  / (PIXELES_X_LADO * escala_dibujo);

    if(juego->estado == ESTADO_PRESENTACION) {
        // Apagamos el offset para que tu presentación ocupe toda la ventana
        offsetX = 0;
        offsetY = 0;

        dibujarFondo(ancho_logico, alto_logico, escala_dibujo);
        dibujarRecuadro(ancho_logico, alto_logico, escala_dibujo);

        int colTetris = (celdas - 12) / 2;
        int colUnlam  = (celdas - 5)  / 2;
        int colPress  = (celdas - 11) / 2;

        textoDibujar(alf, "TETRIS PIXEL", colTetris, filas / 3);
        textoDibujar(alf, "UNLAM", colUnlam, (filas / 3) + 2);

        if(juego->mostrar_press) {
            textoDibujar(alf, "PRESS START", colPress, filas * 2 / 3);
        }

        // Devolvemos el offset a su lugar antes de salir
        offsetX = viejo_offsetX;
        offsetY = viejo_offsetY;
        return;
    }

    if(juego->estado == ESTADO_INGRESO_NOMBRE) {
        offsetX = 0;
        offsetY = 0;

        dibujarFondo(ancho_logico, alto_logico, escala_dibujo);

        int colUsername = (celdas - 8)  / 2;
        int colEnter    = (celdas - 11) / 2;
        int filaNombre  = filas / 2;

        textoDibujar(alf, "USERNAME", colUsername, filas / 3);

        int colNombre = (celdas - juego->nombre_len) / 2;
        if(juego->nombre_len > 0) {
            textoDibujar(alf, juego->nombre_jugador, colNombre, filaNombre);
        }

        if(juego->mostrar_cursor) {
            // El cursor ahora contempla la escala
            int cx = (colNombre + juego->nombre_len) * (PIXELES_X_LADO * escala_dibujo);
            int cy = filaNombre * (PIXELES_X_LADO * escala_dibujo);
            for(int py = 1 * escala_dibujo; py < (PIXELES_X_LADO - 1) * escala_dibujo; py++) {
                for(int px = 1 * escala_dibujo; px < 6 * escala_dibujo; px++) {
                    gbt_dibujar_pixel(cx + px, cy + py, 14);
                }
            }
        }

        textoDibujar(alf, "PRESS ENTER", colEnter, filas * 3 / 4);

        offsetX = viejo_offsetX;
        offsetY = viejo_offsetY;
        return;
    }

    // ==========================================
    // DE ACÁ PARA ABAJO DIBUJA EL JUEGO NORMAL
    // ==========================================

    if(juego->estado == ESTADO_MENU){
        dibujar_texto("CONFIG", 30, 20, AM);

        dibujar_texto("PALETA", 10, 50, VE);
        dibujar_texto("RESOLUCION", 10, 65, VE);
        dibujar_texto("ESCALA", 10, 80, VE);
        dibujar_texto("VELOCIDAD", 10, 95, VE);
        dibujar_texto("JUGAR", 10, 110, VE);

        if(config->paleta == 0) dibujar_texto("RGB", 60, 50, AM);
        else dibujar_texto("VGA", 60, 50, AM);

        if(config->resolucion == RES_CGA) dibujar_texto("CGA", 60, 65, AM);
        else dibujar_texto("VGA", 60, 65, AM);

        if(config->escala == 1) dibujar_texto("STD", 60, 80, AM);
        else dibujar_texto("AMP", 60, 80, AM);

        if(config->velocidad == 1.0f) dibujar_texto("LENTA", 60, 95, AM);
        else if(config->velocidad == 0.5f) dibujar_texto("MEDIA", 60, 95, AM);
        else dibujar_texto("RAPIDA", 60, 95, AM);

        gbt_dibujar_pixel(2, 52 + opcion_menu*15, RO);
    }

    for(int y=FILAS_OCULTAS;y<ALTO_TABLERO;y++)
        for(int x=0;x<ANCHO_TABLERO;x++)
            if(juego->tablero[y][x])
                dibujar(bloque[juego->tablero[y][x]-1],x,y-FILAS_OCULTAS);

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(juego->piezas[juego->pieza_actual][i][j]){
                int dy=juego->posY+i-FILAS_OCULTAS;
                if(dy>=0)
                    dibujar(bloque[juego->pieza_actual],juego->posX+j,dy);
            }

    int panelX = offsetX + ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo + 10;
    int panelY = offsetY;

    dibujar_texto("SCORE", panelX, panelY + 2, AM);
    dibujar_numero(juego->puntaje, panelX, panelY + 10, AM);
    dibujar_texto("NEXT", panelX, panelY + 30, AM);

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(piezas_orig[juego->siguiente][i][j]){
                dibujar(bloque[juego->siguiente], 11 + j, 6 + i);
            }

    int baseX = offsetX;
    int baseY = offsetY;

    int ancho_px = ANCHO_TABLERO * PIXELES_X_LADO * escala_dibujo;
    int alto_px  = ALTO_VISIBLE * PIXELES_X_LADO * escala_dibujo;

    for(int x = 0; x < ancho_px; x++){
        gbt_dibujar_pixel(baseX + x, baseY, VE);
        gbt_dibujar_pixel(baseX + x, baseY + alto_px - 1, VE);
    }

    for(int y = 0; y < alto_px; y++){
        gbt_dibujar_pixel(baseX, baseY + y, VE);
        gbt_dibujar_pixel(baseX + ancho_px - 1, baseY + y, VE);
    }

    if(juego->estado == ESTADO_PAUSA){
        for(int y = offsetY + 55; y < offsetY + 75; y++){
            for(int x = offsetX + 15; x < offsetX + 105; x++){
                gbt_dibujar_pixel(x, y, AZ);
            }
        }
        dibujar_texto("PAUSA", offsetX + 40, offsetY + 62, AM);
    }

    if(juego->estado == ESTADO_GAMEOVER){
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
