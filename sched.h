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

typedef struct scheduler_t{
  uint32_t turno;
  uint32_t last_task[2];
  uint32_t state[sched_max_task];
  uint32_t idx_gdt[sched_max_task];
}scheduler;

extern scheduler sched;
extern uint32_t debug_state;


void sched_init(void);
uint16_t sched_next_task(void);
void change_state_debug(void);

#endif //  __SCHED_H__
