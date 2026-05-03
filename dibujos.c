#include "dibujos.h"

void dibujar(const uint8_t sprite[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    int baseX = oX * (PIXELES_X_LADO + PX_PADDING);
    int baseY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (int y = 0; y < PIXELES_X_LADO; y++) {
        for (int x = 0; x < PIXELES_X_LADO; x++) {

            uint8_t color = sprite[y][x];

            if (color != N) {
                gbt_dibujar_pixel(baseX + x, baseY + y, color);
            }
        }
    }
}
