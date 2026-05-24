/*
 dibujos.h - Cabecera integrada con fuentes, colores y sprites de Tetris.
 */
#ifndef DIBUJOS_H_INCLUDED
#define DIBUJOS_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>

#define PIXELES_X_LADO 8
#define PX_PADDING 0
#define ANCHO_DIGITO 3
#define ALTO_DIGITO  5

// Cada dígito es una matriz de 5 filas x 3 columnas
// 1 = píxel encendido, 0 = transparente

// Declaración de las matrices de caracteres para que otros archivos las vean
extern const uint8_t digitos[10][5][3];
extern const uint8_t letras_score[5][5][3];
extern const uint8_t letras_next[4][5][3];
extern const uint8_t letras_level[5][5][3];
extern const uint8_t letras_lines[5][5][3];
extern const uint8_t letras_statistics[10][5][3];
extern const uint8_t letras_mayus[26][5][3];

// Declaración de variables globales compartidas de renderizado
extern int escala_dibujo;
extern int offsetX;
extern int offsetY;

// Paleta de colores para las piezas y el entorno
#define FONDO 0
#define CE 1 // Celeste
#define AM 2 // Amarillo
#define VI 3 // Violeta
#define VE 4 // Verde
#define RO 5 // Rojo
#define AZ 6 // Azul
#define NA 7 // Naranja
#define N 8 // Negro

// Prototipos de las funciones
void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void dibujar_numero(int numero, uint16_t px, uint16_t py, uint8_t color);
void dibujar_numero_ceros7(int numero, uint16_t px, uint16_t py, uint8_t color);
void dibujar_numero_ceros3(int numero, uint16_t px, uint16_t py, uint8_t color);
void dibujar_texto(const char* txt, uint16_t px, uint16_t py, uint8_t color);

#endif
