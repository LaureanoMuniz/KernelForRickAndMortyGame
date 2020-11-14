/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"
gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] =
        {
            .limit_15_0 = 0x0000,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },
    [GDT_IDX_C0_DESC] = {
            .limit_15_0 = 0xC8FF, //Checkear esto
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x0A,
            .s = 1,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 1,
            .g = 1,
    },
    [GDT_IDX_C3_DESC] = {
            .limit_15_0 = 0xC8FF,
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x0A,
            .s = 1,
            .dpl = 3,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 1,
            .g = 1,
    },
    [GDT_IDX_D0_DESC] = {
            .limit_15_0 = 0xC8FF,
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x02,
            .s = 1,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 1,
            .g = 1,
    },
    [GDT_IDX_D3_DESC] = {
            .limit_15_0 = 0xC8FF,
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x02,
            .s = 1,
            .dpl = 3,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 1,
            .g = 1,
    },
    [GDT_IDX_VID_DESC] = {
            .limit_15_0 = 0x1F3F,
            .limit_19_16 = 0x00,
            .base_15_0 = 0X8000,
            .base_23_16 = 0x0B,
            .base_31_24 = 0x00,
            .type = 0x02,
            .s = 1,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 1,
            .g = 0,
    },
    [GDT_IDX_TSS_INIT] = {
            .limit_15_0 = sizeof(tss_t)-1, //Si se quiere se puede cambiar
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x09,
            .s = 0,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 0,
            .g = 0,
    },
    [GDT_IDX_TSS_IDLE] = {
            .limit_15_0 = sizeof(tss_t)-1, //Same arriba
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x09,
            .s = 0,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 0,
            .g = 0,
    },
    [GDT_IDX_TSS_MORTY] = {
            .limit_15_0 = sizeof(tss_t)-1, //Same arriba
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x09,
            .s = 0,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 0,
            .g = 0,
    },
    [GDT_IDX_TSS_RICK] = {
            .limit_15_0 = sizeof(tss_t)-1, //Same arriba
            .limit_19_16 = 0x00,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .type = 0x09,
            .s = 0,
            .dpl = 0,
            .p = 1,
            .avl = 0,
            .l = 0,
            .db = 0,
            .g = 0,
    },

};

gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};
