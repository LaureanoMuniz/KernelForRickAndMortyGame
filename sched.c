/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"
#include "tss.h"
#include "mmu.h"
#include "screen.h"
scheduler sched;
uint32_t debug_state=0;
void sched_init(void) {
  for(int i=0;i<sched_max_task;i++){
    sched.state[i]=TASK_DEAD;
  }
  for(int i=0;i<sched_max_task;i++){
    sched.idx_gdt[i]=GDT_IDX_TSS_RICK+i;
  }
  sched.last_task[0]=0; //ultima tarea de Rick
  sched.last_task[1]=1; //ultima tarea de Morty
  sched.state[0]=TASK_READY; // Tarea Rick lista
  sched.state[1]=TASK_READY; // Tarea Morty lista
  tss_init_task(GDT_IDX_TSS_RICK, &tss_rick, mmu_next_free_kernel_page(), 
                mmu_init_task_dir(TASK_RICK_DEST_PHY_START, TASK_RICK_SOURCE_PHY_START, TASK_PAGES), 
                TASK_CODE_VIRTUAL, TASK_PAGES);
  tss_init_task(GDT_IDX_TSS_MORTY, &tss_morty, mmu_next_free_kernel_page(), 
                mmu_init_task_dir(TASK_MORTY_DEST_PHY_START, TASK_MORTY_SOURCE_PHY_START, TASK_PAGES), 
                TASK_CODE_VIRTUAL, TASK_PAGES);
  sched.turno = 1; //Proximo turno es de Rick
}

uint16_t sched_next_task(void) {
  if(sched.state[0] == TASK_DEAD || sched.state[1] == TASK_DEAD){
    return GDT_IDX_TSS_IDLE<<3; //selector de tarea
  }
  if(sched.state[sched.last_task[sched.turno]]==TASK_EJEC){
    sched.state[sched.last_task[sched.turno]] = TASK_READY;
  }
  sched.turno++;
  sched.turno=sched.turno%2;
  uint32_t next_task = (sched.last_task[sched.turno] + 2)%sched_max_task; //Candidata a proxima tarea
  while(sched.state[next_task]!=TASK_READY){
    next_task=(next_task+2)%sched_max_task;
  }
  sched.last_task[sched.turno]=next_task;
  sched.state[next_task]=TASK_EJEC;
  return sched.idx_gdt[next_task]<<3; //selector de tarea
}

void sched_desalojar(){
  uint32_t task_a_desalojar=0; 
  while(sched.state[task_a_desalojar]!=TASK_EJEC){ // Si no hay explota
    task_a_desalojar++;
  }
  sched.state[task_a_desalojar] = TASK_DEAD;
  if(task_a_desalojar<2){  //Rick o Morty
    //Termina el juego
  }
  else{ //Mr M
    //Unmapear Mr M
  }
}

void change_state_debug(){
  if(debug_state==0){ //si esta apagado lo prendo
    debug_state=1;
  }
  else if(debug_state==1){ //si esta prendido lo apago
    debug_state=0;
  }
  else{ //si esta mostrando por pantalla dejo de mostrar
    devolver_pantalla();
    debug_state=1;
  }
}