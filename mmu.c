/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

paddr_t proxima_pagina_libre;

void mmu_init(void) {
	proxima_pagina_libre = 0x100000;
}

paddr_t mmu_next_free_kernel_page(void) {
	paddr_t pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE; // +4096
  	return pagina_libre;
}

paddr_t mmu_init_kernel_dir(void) {
	page_directory_entry* directorio = (page_directory_entry*) KERNEL_PAGE_DIR;
	page_table_entry*  tabla_0 = (page_table_entry*) KERNEL_PAGE_TABLE_0;



	for(int i = 0; i<1024; i++){
		directorio[i] = (page_directory_entry){0};
		tabla_0[i] = (page_table_entry){0};
	}

	directorio[0].base = KERNEL_PAGE_TABLE_0 >> 12;
	directorio[0].present = 1;
	directorio[0].read_write = 1;
	directorio[0].user_supervisor = 0;

	for (int i = 0; i<1024; i++){
		tabla_0[i].base = i;
		tabla_0[i].present = 1;
		tabla_0[i].read_write = 1;
		tabla_0[i].user_supervisor = 0;
	}

	return KERNEL_PAGE_DIR;
	

  


}

 void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
 	uint32_t directoryIndex = virt >> 22;
 	uint32_t tableIndex = virt >> 12 & 0x3ff;

 	uint32_t cr3_offset = cr3 >> 12;
 	cr3_offset = cr3_offset << 12;

 	page_directory_entry* pde = (page_directory_entry*) cr3_offset;
 	if (pde[directoryIndex].present != 1){
 		page_table_entry* nueva_tabla = (page_table_entry*) mmu_next_free_kernel_page();

 		for(int i = 0; i<1024 ; i++){
 			nueva_tabla[i] = (page_table_entry){0};
 		}
 		pde[directoryIndex] = (page_directory_entry){0};
 		pde[directoryIndex].base = (uint32_t) nueva_tabla >> 12;
 		pde[directoryIndex].present = (uint8_t) attrs & 1;
 		pde[directoryIndex].read_write = (uint8_t) (attrs & 2) >> 1;
 		pde[directoryIndex].user_supervisor = (uint8_t) (attrs & 4) >> 2;

 	}

 	page_table_entry* pt =  (page_table_entry*) (pde[directoryIndex].base << 12); //Dejo 12 ceros en la parte baja
 	paddr_t phy_offset = phy & ~ 0xfff;
 	pt[tableIndex].base = phy_offset;
 	pt[tableIndex].present = (uint8_t) attrs & 1;
 	pt[tableIndex].read_write = (uint8_t) (attrs & 2) >> 1;
 	pt[tableIndex].user_supervisor = (uint8_t)(attrs & 4) >> 2;
 	tlbflush();




/*
  uint8_t present : 1;
  uint8_t read_write : 1;
  uint8_t user_supervisor : 1;
  uint8_t pwt  : 1;
  uint8_t pcd : 1;
  uint8_t A : 1;
  uint8_t dirty : 1;
  uint8_t pat : 1;
  uint8_t global : 1;
  uint8_t ignorados : 3;
  uint32_t base : 20;
*/

 }


 paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) { //En la declaracion de la catedra decia paddr_t como parametro de retorno
 	uint32_t directoryIndex = virt >> 22;
 	uint32_t tableIndex = virt >> 12 & 0x3ff;
 	uint32_t offset = virt & 0x0fff;
	uint32_t cr3_offset = cr3 >> 12;
 	cr3_offset = cr3_offset << 12;

 	page_directory_entry* pde = (page_directory_entry*) cr3_offset;
 	
 	page_table_entry* pt =  (page_table_entry*) (pde[directoryIndex].base << 12); // Dejo 12 ceros en la parte baja.
 	paddr_t res = (paddr_t) pt[tableIndex].base + (paddr_t) offset;
 	pt[tableIndex] = (page_table_entry){0}; // En el video pide poner 0 solo en el bit de present
	pde[directoryIndex] = (page_directory_entry){0};
 	return res; //Retorna la direccion fisica en la que estaba mapeada la direccion virtual.

 }
// paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t
// pages) {}




