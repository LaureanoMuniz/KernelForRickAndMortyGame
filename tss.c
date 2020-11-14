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
tss_t tss_rick;
tss_t tss_morty;
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

void tss_init_task(paddr_t code_start){
  if(code_start==TASK_RICK_SOURCE_PHY_START){
    define_base_tss(GDT_IDX_TSS_RICK, &tss_rick);
    tss_rick.esp0 = mmu_next_free_kernel_page() + PAGE_SIZE;
    tss_rick.ss0 = GDT_OFF_D0_DESC;
    tss_rick.cr3 = mmu_init_task_dir(TASK_RICK_DEST_PHY_START, code_start, TASK_PAGES);
    tss_rick.eip = TASK_CODE_VIRTUAL;
    tss_rick.eflags = EFLAG;
    tss_rick.es = GDT_OFF_D3_DESC | RPL_3;
    tss_rick.cs = GDT_OFF_C3_DESC | RPL_3;
    tss_rick.ss = GDT_OFF_D3_DESC | RPL_3;
    tss_rick.ds = GDT_OFF_D3_DESC | RPL_3;
    tss_rick.fs = GDT_OFF_D3_DESC | RPL_3;
    tss_rick.gs = GDT_OFF_D3_DESC | RPL_3;
    tss_rick.esp = TASK_CODE_VIRTUAL + PAGE_SIZE*TASK_PAGES;
    tss_rick.ebp = TASK_CODE_VIRTUAL + PAGE_SIZE*TASK_PAGES;
  }
  else{
    define_base_tss(GDT_IDX_TSS_MORTY, &tss_morty);
    tss_morty.esp0 = mmu_next_free_kernel_page() + PAGE_SIZE;
    tss_morty.ss0 = GDT_OFF_D0_DESC;
    tss_morty.cr3 = mmu_init_task_dir(TASK_MORTY_DEST_PHY_START, code_start, TASK_PAGES);
    tss_morty.eip = TASK_CODE_VIRTUAL;
    tss_morty.eflags = EFLAG;
    tss_morty.es = GDT_OFF_D3_DESC | RPL_3;
    tss_morty.cs = GDT_OFF_C3_DESC | RPL_3;
    tss_morty.ss = GDT_OFF_D3_DESC | RPL_3;
    tss_morty.ds = GDT_OFF_D3_DESC | RPL_3;
    tss_morty.fs = GDT_OFF_D3_DESC | RPL_3;
    tss_morty.gs = GDT_OFF_D3_DESC | RPL_3;
    tss_morty.esp = TASK_CODE_VIRTUAL + PAGE_SIZE*TASK_PAGES;
    tss_morty.ebp = TASK_CODE_VIRTUAL + PAGE_SIZE*TASK_PAGES;
  }
}