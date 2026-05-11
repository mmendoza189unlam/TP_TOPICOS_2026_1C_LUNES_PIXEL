#include "presentacion.h"
#include "Texto.h"
#include "dibujos.h"
#include "GBT/gbt.h"
#include <string.h>

static void dibujarFondo(int ancho, int alto)
{
    uint8_t colores[] = {9, 11, 12, 13, 14, 10};
    int filas = alto / 8;
    int cols  = ancho / 8;
    int f, c, py, px;
    for(f = 0; f < filas; f++)
    {
        for(c = 0; c < cols; c++)
        {
            for(py = 0; py < 8; py++)
            {
                for(px = 0; px < 8; px++)
                {
                    gbt_dibujar_pixel(c*8+px, f*8+py, colores[(f+c) % 6]);
                }
            }
        }
    }
}

static void dibujarRecuadro(int ancho, int alto)
{
    int margen = 24;
    int recW = ancho - margen * 2;
    int recH = alto  - margen * 2;
    for(int py = 0; py < recH; py++)
    {
        for(int px = 0; px < recW; px++)
        {
            gbt_dibujar_pixel(margen+px, margen+py, 0);
        }
    }
}

static void iniciarPatron(uint8_t patron[25][40])
{
    uint8_t colores[] = {9, 11, 12, 13, 14, 10};
    for(int f = 0; f < 25; f++)
    {
        for(int c = 0; c < 40; c++)
        {
            patron[f][c] = colores[(f + c) % 6];
        }
    }
}

int presentacionEjecutar(const Alfabeto* alf, int ancho, int alto)
{
    tGBT_Temporizador* timer = gbt_temporizador_crear(0.5);
    int mostrarPress = 1;
    int corriendo    = 1;

    // Calcular celdas disponibles
    int celdas = ancho / (PIXELES_X_LADO + PX_PADDING);
    int filas  = alto  / (PIXELES_X_LADO + PX_PADDING);

    // Calcular columnas centradas para cada texto
    int colTetris = (celdas - 12) / 2;  // "TETRIS PIXEL" = 12 chars
    int colUnlam  = (celdas - 5)  / 2;  // "UNLAM"        = 5 chars
    int colPress  = (celdas - 11) / 2;  // "PRESS START"  = 11 chars

    // Calcular filas
    int filaTetris = filas / 3;
    int filaUnlam  = filaTetris + 2;
    int filaPress  = filas * 2 / 3;

    while(corriendo)
    {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
        {
            gbt_temporizador_destruir(timer);
            return 0;
        }
        if(tecla == GBTK_ENTER)
        {
            corriendo = 0;
        }

        if(gbt_temporizador_consumir(timer))
        {
            mostrarPress = !mostrarPress;
        }

        dibujarFondo(ancho, alto);
        dibujarRecuadro(ancho, alto);

        textoDibujar(alf, "TETRIS PIXEL", colTetris, filaTetris);
        textoDibujar(alf, "UNLAM",        colUnlam,  filaUnlam);

        if(mostrarPress)
        {
            textoDibujar(alf, "PRESS START", colPress, filaPress);
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(timer);
    return 1;
}

int usernameEjecutar(const Alfabeto* alf, char* nombreOut, int ancho, int alto)
{
    char nombre[USERNAME_MAX_LEN + 1];
    memset(nombre, 0, sizeof(nombre));
    int len = 0;

    tGBT_Temporizador* timerCursor = gbt_temporizador_crear(0.5);
    int mostrarCursor = 1;
    int corriendo     = 1;

    // Calcular celdas disponibles
    int celdas = ancho / (PIXELES_X_LADO + PX_PADDING);
    int filas  = alto  / (PIXELES_X_LADO + PX_PADDING);

    // Posiciones fijas
    int colUsername = (celdas - 8)  / 2;  // "USERNAME"   = 8 chars
    int colEnter    = (celdas - 11) / 2;  // "PRESS ENTER" = 11 chars
    int filaNombre  = filas / 2;
    int filaEnter   = filas * 3 / 4;

    while(corriendo)
    {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ESCAPE)
        {
            gbt_temporizador_destruir(timerCursor);
            return 0;
        }

        if(tecla >= GBTK_a && tecla <= GBTK_z && len < USERNAME_MAX_LEN)
        {
            nombre[len] = 'A' + (tecla - GBTK_a);
            len++;
        }

        if(tecla == GBTK_RETROCESO && len > 0)
        {
            len--;
            nombre[len] = '\0';
        }

        if(tecla == GBTK_ENTER && len > 0)
        {
            corriendo = 0;
        }

        if(gbt_temporizador_consumir(timerCursor))
        {
            mostrarCursor = !mostrarCursor;
        }

        dibujarFondo(ancho, alto);
        //dibujarRecuadro(ancho, alto);

        textoDibujar(alf, "USERNAME", colUsername, filas / 3);

        // Nombre centrado — cambia con cada letra
        int colNombre = (celdas - len) / 2;
        if(len > 0)
            textoDibujar(alf, nombre, colNombre, filaNombre);

        // Cursor justo después del último carácter
        if(mostrarCursor)
        {
            int cursorCol = colNombre + len;
            int cx = cursorCol * (PIXELES_X_LADO + PX_PADDING);
            int cy = filaNombre * (PIXELES_X_LADO + PX_PADDING);
            int py, px;
            for(py = 1; py < PIXELES_X_LADO - 1; py++)
            {
                for(px = 1; px < 6; px++)
                {
                    gbt_dibujar_pixel(cx + px, cy + py, 14);
                }
            }
        }

        textoDibujar(alf, "PRESS ENTER", colEnter, filaEnter);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(timerCursor);
    strncpy(nombreOut, nombre, USERNAME_MAX_LEN);
    nombreOut[USERNAME_MAX_LEN] = '\0';
    return 1;
}
