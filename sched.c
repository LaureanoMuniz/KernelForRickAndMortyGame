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
#include "i386.h"

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
  sched.turno = 1; //Proximo turno es de Rick

}

uint16_t sched_next_task(void) {
  if(sched.state[0] == TASK_DEAD || sched.state[1] == TASK_DEAD || juego.cant_Megasemilla==0){
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
  if(next_task>1){ // es Mr M
    juego.max_move_Mr_M[next_task-2] = maximo(juego.max_move_Mr_M[next_task-2]-1,2);
  }
  sched.state[next_task]=TASK_EJEC;
  sched.reloj[next_task]=(sched.reloj[next_task]+1)%4;
  return sched.idx_gdt[next_task]<<3; //selector de tarea
}

void sched_desalojar(){
  uint32_t task_a_desalojar=0; 
  while(sched.state[task_a_desalojar]!=TASK_EJEC){ // Si no hay explota
    task_a_desalojar++;
  }
  sched.state[task_a_desalojar] = TASK_DEAD;
  if(task_a_desalojar<2){  //Rick o Morty
    //Termino el juego
  }
  else{ //Mr M
    //Unmapear Mr M
    uint32_t slot_tarea = (task_a_desalojar-2)/2;
    vaddr_t virtual_Mr_M = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
    mmu_unmap_page(rcr3(), virtual_Mr_M);
    mmu_unmap_page(rcr3(), virtual_Mr_M + PAGE_SIZE);
  
  }
  saltar_idle();
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