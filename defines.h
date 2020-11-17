/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__
/* MMU */
/* -------------------------------------------------------------------------- */

#define MMU_P (1 << 0)
#define MMU_W (1 << 1)
#define MMU_U (1 << 2)

#define PAGE_SIZE 4096

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40

// X Columnas
#define SIZE_M 80

// Atributo de Interruption Gate 
#define ATTR_INT_GATE_0 0x8E00
#define ATTR_INT_GATE_3 0XEE00
// EFLAGS
#define EFLAG 0x202

#define RPL_3 3
/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC 0
#define GDT_IDX_C0_DESC   10
#define GDT_IDX_D0_DESC   11
#define GDT_IDX_C3_DESC   12
#define GDT_IDX_D3_DESC   13
#define GDT_IDX_VID_DESC  14

#define GDT_IDX_TSS_INIT  20
#define GDT_IDX_TSS_IDLE  21
#define GDT_IDX_TSS_RICK  22
#define GDT_IDX_TSS_MORTY 23


#define GDT_COUNT         35
/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_C0_DESC   (GDT_IDX_C0_DESC << 3)
#define GDT_OFF_D0_DESC   (GDT_IDX_D0_DESC << 3)
#define GDT_OFF_C3_DESC   (GDT_IDX_C3_DESC << 3)
#define GDT_OFF_D3_DESC   (GDT_IDX_D3_DESC << 3)
#define GDT_OFF_VID_DESC  (GDT_IDX_VID_DESC << 3)

#define GDT_OFF_TSS_INIT  (GDT_IDX_TSS_INIT << 3)
#define GDT_OFF_TSS_IDLE  (GDT_IDX_TSS_IDLE << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */

// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x01D00000
#define TASK_PAGES        4

/* Direcciones fisicas de codigos */
#define TASK_IDLE_PHY_START 0x00018000
#define TASK_RICK_SOURCE_PHY_START 0x00010000
#define TASK_MORTY_SOURCE_PHY_START 0x00014000

#define TASK_RICK_DEST_PHY_START 0x01D00000
#define TASK_MORTY_DEST_PHY_START 0x01D04000

/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<X>_PHY_START.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

#endif //  __DEFINES_H__
