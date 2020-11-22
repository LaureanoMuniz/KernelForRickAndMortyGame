/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "prng.h"
#include "types.h"
#include "defines.h"
#include "screen.h"
#include "tss.h"
#include "sched.h"
#include "mmu.h"
#include "i386.h"

juego_type juego;

void colocar_Megasemillas(){
    for(int i=0;i<CANT_MAX_MEGASEMILLAS;i++){
        bool esDistinto=false;
        while(!esDistinto){
            juego.posicion_Megasemillas[i].x=rand()%SIZE_M;
            juego.posicion_Megasemillas[i].y=rand()%SIZE_N;
            esDistinto=true;
            for(int j=0;j<i;j++){
                if(juego.posicion_Megasemillas[i].x == juego.posicion_Megasemillas[j].x &&
                   juego.posicion_Megasemillas[i].y == juego.posicion_Megasemillas[j].y){
                       esDistinto=false;
                   }
            }
        } 
    }
    colocar_Megasemillas_Pantalla();
}

void game_init(void) {
    //puntajes
    juego.puntajes[0]=0; //puntaje de Rick
    juego.puntajes[1]=0; //puntaje de Morty
    juego.color_players[0] = C_FG_BLUE | C_BG_GREEN;
    juego.color_players[1] = C_FG_RED | C_BG_GREEN;
    //colocar semillas
    juego.cant_Megasemilla=CANT_MAX_MEGASEMILLAS;
    colocar_Megasemillas();
    
    // iniciar tareas Rick y Morty
    sched.state[0]=TASK_READY; // Tarea Rick lista
    sched.state[1]=TASK_READY; // Tarea Morty lista
    tss_init_task(GDT_IDX_TSS_RICK, &tss_rick, mmu_next_free_kernel_page(), 
                mmu_init_task_dir(TASK_RICK_DEST_PHY_START, TASK_RICK_SOURCE_PHY_START, TASK_PAGES), 
                TASK_CODE_VIRTUAL, TASK_PAGES);
    tss_init_task(GDT_IDX_TSS_MORTY, &tss_morty, mmu_next_free_kernel_page(), 
                mmu_init_task_dir(TASK_MORTY_DEST_PHY_START, TASK_MORTY_SOURCE_PHY_START, TASK_PAGES), 
                TASK_CODE_VIRTUAL, TASK_PAGES);
    
    //crear gdts para Mr M y pages del stack de nivel 0
    for(int i=GDT_IDX_TSS_MORTY+1;i<GDT_IDX_TSS_MORTY+21;i++){
        gdt[i]=gdt_tarea_Mr_M;
    }
    for(int i=0;i<20;i++){
        juego.page_stack0_Mr_M[i] = mmu_next_free_kernel_page();
    }

}

uint32_t buscar_semilla(uint32_t x, uint32_t y){
    for(int i=0;i<CANT_MAX_MEGASEMILLAS;i++){
        if(x==juego.posicion_Megasemillas[i].x && y == juego.posicion_Megasemillas[i].y){
            return i;
        }
    }
    return -1;
}

uint32_t servicio_meeseks(uint32_t code, uint32_t x, uint32_t y){
    //aca poner restricciones
    if(sched.last_task[sched.turno]>1){ //no es Rick ni Morty
        sched_desalojar();
    }
    uint32_t nueva_tarea=sched.turno;
    for(int i=sched.turno;i<sched_max_task;i+=2){
        if(sched.state[i]==TASK_DEAD){
            nueva_tarea=i;
            break;
        }
    }
    if(nueva_tarea==sched.turno){ // no hay lugares
        return 0;
    }
    uint32_t indice_semilla = buscar_semilla(x,y);
    if(indice_semilla!=(uint32_t)-1){ // hay una semilla
        juego.cant_Megasemilla--;
        juego.posicion_Megasemillas[indice_semilla].x=-1;
        juego.posicion_Megasemillas[indice_semilla].y=-1;
        juego.puntajes[sched.turno]+=425;
        actualizar_puntajes();
        print(" ", x, y+1, C_BG_GREEN);
        return 0;
    }
    //hay lugar y no hay semilla
    juego.posiciones_Mr_M[nueva_tarea-2].x=x;
    juego.posiciones_Mr_M[nueva_tarea-2].y=y;

    print("K", x, y+1, juego.color_players[sched.turno]);
    uint32_t slot_tarea = (nueva_tarea-2)/2; //Que numero de tarea es del jugador
    paddr_t dir_dest = MAP_PHY_START + y*(PAGE_SIZE*2)*VIDEO_COLS + x*(PAGE_SIZE*2); //es columna
    vaddr_t dir_source = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
    
    // se mapea las direcciones necesarias
    mmu_map_page(rcr3(), dir_source, dir_dest, 7);
    mmu_map_page(rcr3(), dir_source + PAGE_SIZE, dir_dest + PAGE_SIZE, 7);
    
    // se copian el codigo de la tarea
    uint8_t* copy_dest = (uint8_t*) dir_source;
    uint8_t* copy_source = (uint8_t*) code;
    for(int i=0;i<PAGE_SIZE;i++){
        copy_dest[i]=copy_source[i];
    }

    //se inicia la tss
    tss_init_task(sched.idx_gdt[nueva_tarea], &tss_Mr_M[nueva_tarea-2], 
                  juego.page_stack0_Mr_M[nueva_tarea-2], rcr3(), dir_source,2);

    sched.state[nueva_tarea]=TASK_READY;
    

    return dir_source;
}