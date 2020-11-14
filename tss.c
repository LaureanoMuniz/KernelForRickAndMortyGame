/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"

tss_t tss_initial = {0};
tss_t tss_idle = {
  //Chequear
    .esp0 = KERNEL_STACK, 
    .ss0 = GDT_OFF_D0_DESC,
    .cr3 = KERNEL_PAGE_DIR,
    .eip = TASK_IDLE_PHY_START,
    .eflags = EFLAG,
    .es = GDT_OFF_D0_DESC,
    .cs = GDT_OFF_C0_DESC,
    .ss = GDT_OFF_D0_DESC,
    .ds = GDT_OFF_D0_DESC,
    .fs = GDT_OFF_D0_DESC,
    .gs = GDT_OFF_D0_DESC,
    .esp = KERNEL_STACK,
    .ebp = KERNEL_STACK,
};


void tss_init(void) {
  gdt[GDT_OFF_TSS_INIT].base_15_0 = (uint32_t) &tss_initial;
  gdt[GDT_OFF_TSS_INIT].base_23_16 = (uint32_t) &tss_initial >> 16; 
  gdt[GDT_OFF_TSS_INIT].base_31_24 = (uint32_t) &tss_initial >> 24;

}
void tss_idle_init(void){
  gdt[GDT_OFF_TSS_IDLE].base_15_0 = (uint32_t) &tss_idle;
  gdt[GDT_OFF_TSS_IDLE].base_23_16 = (uint32_t) &tss_idle >> 16; 
  gdt[GDT_OFF_TSS_IDLE].base_31_24 = (uint32_t) &tss_idle >> 24;
}