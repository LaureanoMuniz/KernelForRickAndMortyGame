/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#define TASK_DEAD 0
#define TASK_READY 1
#define TASK_EJEC 2
#define sched_max_task 22

uint32_t turno;
uint32_t last_task[2];

void sched_init();
uint16_t sched_next_task();

extern uint32_t sched_state[sched_max_task]; // 0 -> muerta o no cargada, 1 -> ready, 2-> ejecutando 
extern uint32_t sched_idx_gdt[sched_max_task]; // indice de la tarea i en la gdt
#endif //  __SCHED_H__
