#include "dibujos.h"

void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    uint16_t offsetX = oX * (PIXELES_X_LADO + PX_PADDING);
    uint16_t offsetY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (uint16_t y = 0; y < PIXELES_X_LADO; y++) {
        for (uint16_t x = 0; x < PIXELES_X_LADO; x++) {

            gbt_dibujar_pixel(offsetX + x, offsetY + y, dibujo[y][x]);
        }
    }
}
