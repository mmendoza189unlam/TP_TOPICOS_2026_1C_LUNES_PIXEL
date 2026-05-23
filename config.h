#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/* ----- Resoluciones lógicas disponibles ----- */
typedef enum {
    RES_CGA,
    RES_VGA
} ResolucionLogica;

/* ----- Configuración general del juego ----- */

typedef struct {
    int paleta;
    int resolucion;
    int escala;
    float velocidad;
} Configuracion;

/* ----- Variable global (definida en config.c)-----*/
extern Configuracion config;

/* ----- Funciones -----*/
void cargar_config(void);
void guardar_config(void);
void obtener_resolucion_logica(int resolucion, int *ancho, int *alto);
void recrear_ventana(void);

#endif // CONFIG_H_INCLUDED
