/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
#include "tss.h"
uint32_t sched_state[sched_max_task];
uint32_t sched_idx_gdt[sched_max_task];
void sched_init(void) {
  for(int i=0;i<sched_max_task;i++){
    sched_state[i]=TASK_DEAD;
  }
  for(int i=0;i<sched_max_task;i++){
    sched_idx_gdt[i]=GDT_IDX_TSS_RICK+i;
  }
  last_task[0]=0; //ultima tarea de Rick
  last_task[1]=1; //ultima tarea de Morty
  sched_state[0]=TASK_READY; // Tarea Rick lista
  sched_state[1]=TASK_READY; // TArea Morty lista
  tss_init_task(TASK_RICK_SOURCE_PHY_START);
  tss_init_task(TASK_MORTY_SOURCE_PHY_START);
  turno = 1;
}

uint16_t sched_next_task(void) {
  if(sched_state[0] == TASK_DEAD || sched_state[1] == TASK_DEAD){
    return GDT_IDX_TSS_IDLE<<3; //selector de tarea
  }
  if(sched_state[last_task[turno]]==TASK_EJEC){
    sched_state[last_task[turno]] = TASK_READY;
  }
  turno++;
  turno=turno%2;
  uint32_t next_task = (last_task[turno] + 2)%sched_max_task; //Candidata a proxima tarea
  while(sched_state[next_task]!=TASK_READY){
    next_task=(next_task+2)%sched_max_task;
  }
  sched_state[next_task]=TASK_EJEC;
  return sched_idx_gdt[next_task]<<3; //selector de tarea
}

void sched_desalojar(){
  uint32_t task_a_desalojar=0; 
  while(sched_state[task_a_desalojar]!=TASK_EJEC){
    task_a_desalojar++;
  }
  sched_state[task_a_desalojar] = TASK_DEAD;
  if(task_a_desalojar<2){  //Rick o Morty
    //Termina el juego
  }
  else{ //Mr M
    //Unmapear Mr M
  }
}