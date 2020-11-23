/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
#include "stdint.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
  uint8_t c;
  uint8_t a;
} ca;

ca copia_de_pantalla[50][80];

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr);

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr);

void screen_draw_layout(void);

void imprimir_debug(uint32_t id_tarea, uint8_t gs, uint8_t fs, uint8_t es, uint8_t ds, 
                    uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t esp_kernel, 
                    uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax,
                    uint32_t err,uint32_t eip, uint8_t cs, uint32_t eflags, uint32_t esp, uint8_t ss);

void copiar_pantalla(void);

void devolver_pantalla(void);

void actualizar_puntajes(void);

void actualizar_pantalla(void);
#endif //  __SCREEN_H__
