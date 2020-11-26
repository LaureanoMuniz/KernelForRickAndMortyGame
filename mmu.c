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

 	page_table_entry* pt =  (page_table_entry*) (pde[directoryIndex].base << 12);
 	paddr_t phy_offset = phy & ~ 0xfff;
 	pt[tableIndex].base = (uint32_t) phy_offset >> 12;
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
 	if(pde[directoryIndex].present != 1){
		 return 0; //Preguntar que returnear
	}
 	page_table_entry* pt =  (page_table_entry*) (pde[directoryIndex].base << 12); // Dejo 12 ceros en la parte baja.
 	paddr_t res = ((paddr_t) pt[tableIndex].base << 12) + (paddr_t) offset; //+ offset hace falta? quizas devuelva basura
 	pt[tableIndex] = (page_table_entry){0}; // En el video pide poner 0 solo en el bit de present
	tlbflush();
 	return res; //Retorna la direccion fisica en la que estaba mapeada la direccion virtual.

 }


paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t pages) {
	paddr_t cr3_new = mmu_next_free_kernel_page();
	paddr_t cr3 = rcr3();
	page_directory_entry* directorio = (page_directory_entry*) cr3_new;
	
	for(int i = 0; i<1024; i++){
		directorio[i] = (page_directory_entry){0};
	}
	
	for(vaddr_t i = 0; i<1024; i++){
		mmu_map_page(cr3_new, i<<12, (paddr_t) i << 12, 3); 
	}
	for(size_t i = 0; i < pages; i++){
		mmu_map_page(cr3_new, TASK_CODE_VIRTUAL + i*0x1000, phy_start + i*0x1000, 7); //Chequear
		mmu_map_page(cr3, TASK_CODE_VIRTUAL + i*0x1000, phy_start + i*0x1000, 3); //user o no?
	}
	uint8_t* codigo = (uint8_t*)code_start;
	uint8_t* copiar = (uint8_t*)TASK_CODE_VIRTUAL;
	for(uint32_t i=0; i<PAGE_SIZE*pages; i++){
		copiar[i]=codigo[i];
	}
	for(size_t i = 0; i < pages; i++){
		mmu_unmap_page(cr3, TASK_CODE_VIRTUAL + i*0x1000);
	}
	return cr3_new;
}

void move_code_Mr_M(paddr_t orig, paddr_t dest){
	//mapear las paginas necesarias del mapa (donde se encuentra el Mr M y donde quiero que este)
	//Se mapean con Identity Mapping
	//Puede ser nivel supervisor o user, solo se llama cuando esta ejecutando con nivel 0
	mmu_map_page(rcr3(), orig, orig, 3);
	mmu_map_page(rcr3(), orig + PAGE_SIZE, orig + PAGE_SIZE, 3);
	mmu_map_page(rcr3(), dest, dest, 3);
	mmu_map_page(rcr3(), dest + PAGE_SIZE, dest + PAGE_SIZE, 3);

	uint8_t* orig_copy = (uint8_t*) orig;
	uint8_t* dest_copy = (uint8_t*) dest;
	for(int i = 0; i < 2*PAGE_SIZE;i++){
		dest_copy[i]=orig_copy[i];
	}

	mmu_unmap_page(rcr3(), orig);
	mmu_unmap_page(rcr3(), orig + PAGE_SIZE);
	mmu_unmap_page(rcr3(), dest);
	mmu_unmap_page(rcr3(), dest + PAGE_SIZE);
 
}


