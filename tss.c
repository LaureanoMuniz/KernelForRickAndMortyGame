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
tss_t tss_rick;
tss_t tss_morty;
tss_t tss_Mr_M[CANT_MAX_MR_M];

void define_base_tss(int index, tss_t* task){
  gdt[index].base_15_0 = (uint32_t) task;
  gdt[index].base_23_16 = (uint32_t) task >> 16;
  gdt[index].base_31_24 = (uint32_t) task >> 24;
}

void tss_init(void) {
  define_base_tss(GDT_IDX_TSS_INIT,&tss_initial);
}
void tss_idle_init(void){
  define_base_tss(GDT_IDX_TSS_IDLE, &tss_idle);
}

void tss_init_task(int gdt_idx, tss_t* tss, paddr_t kernel_page, paddr_t cr3, 
                    vaddr_t task_code, uint32_t pages){
  define_base_tss(gdt_idx, tss);
  tss->esp0 = kernel_page + PAGE_SIZE;
  tss->ss0 = GDT_OFF_D0_DESC;
  tss->cr3 = cr3;
  tss->eip = task_code;
  tss->eflags = EFLAG;
  tss->es = GDT_OFF_D3_DESC | RPL_3;
  tss->cs = GDT_OFF_C3_DESC | RPL_3;
  tss->ss = GDT_OFF_D3_DESC | RPL_3;
  tss->ds = GDT_OFF_D3_DESC | RPL_3;
  tss->fs = GDT_OFF_D3_DESC | RPL_3;
  tss->gs = GDT_OFF_D3_DESC | RPL_3;
  tss->esp = task_code + PAGE_SIZE*pages;
  tss->ebp = task_code + PAGE_SIZE*pages;
}