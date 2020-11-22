/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"
#define CANT_MAX_MR_M 20
#define CANT_MAX_MEGASEMILLAS 40
typedef enum e_task_type {
  Rick = 1,
  Morty = 2,
  Meeseeks = 3,
} task_type_t;
typedef struct posicion{
  uint32_t x; // Columna
  uint32_t y; // FIla
}pos;

typedef struct s_juego{
  pos posiciones_Mr_M[CANT_MAX_MR_M];
  pos posicion_Megasemillas[CANT_MAX_MEGASEMILLAS];
  uint32_t cant_Megasemilla;
  uint32_t puntajes[2];
  vaddr_t page_stack0_Mr_M[CANT_MAX_MR_M];
  uint8_t color_players[2];
}juego_type;

extern juego_type juego;

void game_init(void);
void colocar_Megasemillas(void);
uint32_t servicio_meeseks(uint32_t code, uint32_t x, uint32_t y);

#endif //  __GAME_H__
