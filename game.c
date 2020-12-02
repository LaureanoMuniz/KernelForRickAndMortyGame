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
uint32_t max_esp_task[sched_max_task];
uint32_t min_esp_task[sched_max_task];
uint32_t maximo(uint32_t n,uint32_t m){
    if(n>m){
        return n;
    }
    return m;
    
}
uint32_t minimo(uint32_t n, uint32_t m){
    if(n<m){
        return n;
    }
    return m;
}
int32_t absoluto(int32_t x){
    if(x<0){
        return -x;
    }
    return x;
}
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
}

void game_init(void) {
    //puntajes
    juego.puntajes[0]=0; //puntaje de Rick
    juego.puntajes[1]=0; //puntaje de Morty
    juego.color_players[0] = C_FG_RED | C_BG_GREEN;
    juego.color_players[1] = C_FG_BLUE | C_BG_GREEN;
    //colocar semillas
    juego.cant_Megasemilla=CANT_MAX_MEGASEMILLAS;
    colocar_Megasemillas();
    
    // iniciar tareas Rick y Morty
    sched.state[0]=TASK_READY; // Tarea Rick lista
    sched.state[1]=TASK_READY; // Tarea Morty lista
    juego.cr3_players[0]= mmu_init_task_dir(TASK_RICK_DEST_PHY_START, TASK_RICK_SOURCE_PHY_START, TASK_PAGES);
    juego.cr3_players[1]= mmu_init_task_dir(TASK_MORTY_DEST_PHY_START, TASK_MORTY_SOURCE_PHY_START, TASK_PAGES);
    tss_init_task(GDT_IDX_TSS_RICK, &tss_rick, mmu_next_free_kernel_page(), 
                juego.cr3_players[0], TASK_CODE_VIRTUAL, TASK_PAGES);
    tss_init_task(GDT_IDX_TSS_MORTY, &tss_morty, mmu_next_free_kernel_page(), 
                 juego.cr3_players[1], TASK_CODE_VIRTUAL, TASK_PAGES);
    
    //crear gdts para Mr M y pages del stack de nivel 0
    for(int i=GDT_IDX_TSS_MORTY+1;i<GDT_IDX_TSS_MORTY+21;i++){
        gdt[i]=gdt_tarea_Mr_M;
    }
    for(int i=0;i<CANT_MAX_MR_M;i++){
        juego.uso_portal_gun[i]=0;
        juego.page_stack0_Mr_M[i] = mmu_next_free_kernel_page();
    }

    //Maximos esp
    max_esp_task[0]=TASK_CODE_VIRTUAL+4*PAGE_SIZE;
    max_esp_task[1]=TASK_CODE_VIRTUAL+4*PAGE_SIZE;
    min_esp_task[0]=TASK_CODE_VIRTUAL;
    min_esp_task[1]=TASK_CODE_VIRTUAL;
    for(int i=2;i<sched_max_task;i++){
      uint32_t slot_tarea = (i-2)/2;
      max_esp_task[i]= (slot_tarea+1)*(2*PAGE_SIZE)+ NEW_TASKS_VIRTUAL_START;
      min_esp_task[i]= (slot_tarea)*(2*PAGE_SIZE)+ NEW_TASKS_VIRTUAL_START;
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
    uint32_t dentro_del_mapa = (x < SIZE_M) && (y < SIZE_N);
    uint32_t dentro_de_nivel_3 = (TASK_CODE_VIRTUAL<=code) && (code<TASK_CODE_VIRTUAL+4*PAGE_SIZE);
    if(sched.last_task[sched.turno]>1 || !dentro_del_mapa || !dentro_de_nivel_3){ //no es Rick ni Morty
        sched_desalojar(); //desalojar tarea actual
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
        return 0;
    }
    //hay lugar y no hay semilla
    sched.reloj[nueva_tarea] = 0;
    juego.uso_portal_gun[nueva_tarea-2] = 0;
    juego.max_move_Mr_M[nueva_tarea-2] = 16; 
    juego.posiciones_Mr_M[nueva_tarea-2].x=x;
    juego.posiciones_Mr_M[nueva_tarea-2].y=y;

    
    uint32_t slot_tarea = (nueva_tarea-2)/2; //Que numero de tarea es del jugador
    paddr_t dir_dest = MAP_PHY_START + y*(PAGE_SIZE*2)*VIDEO_COLS + x*(PAGE_SIZE*2);
    vaddr_t dir_source = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
    
    // se mapea las direcciones necesarias
    mmu_map_page(rcr3(), dir_source, dir_dest, 7);
    mmu_map_page(rcr3(), dir_source + PAGE_SIZE, dir_dest + PAGE_SIZE, 7);
    
    // se copian el codigo de la tarea
    uint8_t* copy_dest = (uint8_t*) dir_source;
    uint8_t* copy_source = (uint8_t*) code;
    uint32_t limite = TASK_CODE_VIRTUAL + 4*PAGE_SIZE;
    for(uint32_t i=0;i<minimo(PAGE_SIZE,limite-code);i++){
        copy_dest[i]=copy_source[i];
    }

    //se inicia la tss
    tss_init_task(sched.idx_gdt[nueva_tarea], &tss_Mr_M[nueva_tarea-2], 
                  juego.page_stack0_Mr_M[nueva_tarea-2], rcr3(), dir_source, 2);

    sched.state[nueva_tarea]=TASK_READY;
    

    return dir_source;
}

uint32_t servicio_move(int32_t x, int32_t y){
    uint32_t rick_O_morty = (sched.last_task[sched.turno]<2);
    if(rick_O_morty){
        sched_desalojar();
    }
    //es tarea Mr M
    uint32_t tarea = sched.last_task[sched.turno];
    uint32_t slot_tarea = (tarea-2)/2;
    if(juego.max_move_Mr_M[tarea-2]/2<(uint32_t)absoluto(x)+(uint32_t)absoluto(y)){ // Si no puede moverse returneo 0 
        return 0;
    }
    if(x==0 && y==0){ // si no se mueve, devuelvo que fue efectivo
        return 1;
    }
    x = ((x%SIZE_M)+SIZE_M)%SIZE_M;
    y = ((y%SIZE_N)+SIZE_N)%SIZE_N;

    int32_t pos_final_x = ((int32_t)juego.posiciones_Mr_M[tarea-2].x+x)%SIZE_M;
    int32_t pos_final_y = ((int32_t)juego.posiciones_Mr_M[tarea-2].y+y)%SIZE_N;
    
    uint32_t indice_semilla = buscar_semilla(pos_final_x, pos_final_y);
    if(indice_semilla!=(uint32_t)-1){ // hay una semilla
        juego.cant_Megasemilla--;
        juego.posicion_Megasemillas[indice_semilla].x=-1;
        juego.posicion_Megasemillas[indice_semilla].y=-1;
        juego.puntajes[sched.turno]+=425;
        sched_desalojar();
    }
    paddr_t dir_source = MAP_PHY_START + juego.posiciones_Mr_M[tarea-2].y*(PAGE_SIZE*2)*VIDEO_COLS + 
                         juego.posiciones_Mr_M[tarea-2].x*(PAGE_SIZE*2);
    paddr_t dir_dest = MAP_PHY_START + pos_final_y*(PAGE_SIZE*2)*VIDEO_COLS + 
                       pos_final_x*(PAGE_SIZE*2);
    // Copio codigo 
    move_code_Mr_M(dir_source, dir_dest);
    vaddr_t map_source = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
    mmu_unmap_page(rcr3(), map_source);
    mmu_unmap_page(rcr3(), map_source + PAGE_SIZE);
    mmu_map_page(rcr3(), map_source, dir_dest, 7);
    mmu_map_page(rcr3(), map_source + PAGE_SIZE, dir_dest + PAGE_SIZE, 7);

    juego.posiciones_Mr_M[tarea-2].x= pos_final_x;
    juego.posiciones_Mr_M[tarea-2].y= pos_final_y;
    return 1;

}

int32_t calcular_distancia(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
    return absoluto(x1-x2)+absoluto(y1-y2);
}
uint32_t semilla_mas_cercana(){
    int32_t menor_dist = 1000; // Numero mayor a 80+40
    uint32_t semilla_menor_dist;
    uint32_t tarea = sched.last_task[sched.turno];
    for(int i=0;i<CANT_MAX_MEGASEMILLAS;i++){
        if(juego.posicion_Megasemillas[i].x != (uint32_t)-1){ //la semilla esta en el mapa
            int32_t dist=calcular_distancia(juego.posicion_Megasemillas[i].x, juego.posicion_Megasemillas[i].y,
                                           juego.posiciones_Mr_M[tarea-2].x, juego.posiciones_Mr_M[tarea-2].y);
            if(dist<menor_dist){
                menor_dist=dist;
                semilla_menor_dist=i;
            }
        }   
    }
    return semilla_menor_dist;
}
int8_t servicio_look_x(){
    uint8_t x;
    uint32_t tarea = sched.last_task[sched.turno];
    if(tarea<2){ //Me fijo si la tarea actual es Rick O Morty 
        x=-1;
        return x;
    }
    uint32_t semilla_menor_dist= semilla_mas_cercana();
    x = (int8_t) juego.posicion_Megasemillas[semilla_menor_dist].x -(int8_t) juego.posiciones_Mr_M[tarea-2].x;
    return x;
}
int8_t servicio_look_y(){
    uint8_t y;
    uint32_t tarea = sched.last_task[sched.turno];
    if(tarea<2){ //Me fijo si la tarea actual es Rick O Morty 
        y=-1;
        return y;
    }
    uint32_t semilla_menor_dist= semilla_mas_cercana();
    y = (int8_t) juego.posicion_Megasemillas[semilla_menor_dist].y - (int8_t) juego.posiciones_Mr_M[tarea-2].y;
    return y;
}

uint32_t random_task_rival(){
    uint32_t cant_tareas_disponibles = 0;
    uint32_t otro_jugador = (sched.turno+1)%2;
    for(int i=otro_jugador+2;i<sched_max_task;i+=2){
        if(sched.state[i]==TASK_READY){
            cant_tareas_disponibles++;
        }
    }
    if(cant_tareas_disponibles==0){
        return otro_jugador; 
    }
    uint32_t random_task=otro_jugador;
    uint32_t random_index = rand() % cant_tareas_disponibles;
    uint32_t contador = 0;
    for(int i=otro_jugador+2;i<sched_max_task;i+=2){
        if(sched.state[i]==TASK_READY && contador==random_index){
            random_task=i;
            break;
        }
        else if(sched.state[i]==TASK_READY){
            contador++;
        }
    }
    return random_task;
}

void servicio_portal_gun(){
    uint32_t tarea = sched.last_task[sched.turno];
    if(tarea < 2){ // es Rick o Morty
        return;
    }
    if(juego.uso_portal_gun[tarea-2]){
        return;
    }
    juego.uso_portal_gun[tarea-2] = 1;
    uint32_t otro_jugador=(sched.turno+1)%2;
    uint32_t random_task=random_task_rival();
    if(otro_jugador==random_task){ //No tenia tareas Mr M
        return;
    }
    uint32_t random_pos_x = rand() % SIZE_M;
    uint32_t random_pos_y = rand() % SIZE_N;
    if(random_pos_x == juego.posiciones_Mr_M[random_task-2].x &&
       random_pos_y == juego.posiciones_Mr_M[random_task-2].y){//misma posicion
            return;
       }
    

    uint32_t indice_semilla = buscar_semilla(random_pos_x, random_pos_y);
    uint32_t slot_tarea = (random_task-2)/2;
   
    if(indice_semilla!=(uint32_t)-1){ // hay una semilla
        juego.cant_Megasemilla--;
        juego.posicion_Megasemillas[indice_semilla].x=-1;
        juego.posicion_Megasemillas[indice_semilla].y=-1;
        juego.puntajes[otro_jugador]+=425;
        //Asimila la Megasemilla por ende desaparece
        sched.state[random_task] = TASK_DEAD;
        vaddr_t virtual_Mr_M = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
        mmu_unmap_page(juego.cr3_players[otro_jugador], virtual_Mr_M);
        mmu_unmap_page(juego.cr3_players[otro_jugador], virtual_Mr_M + PAGE_SIZE);
        return ;    
    }
    

    paddr_t dir_source = MAP_PHY_START + juego.posiciones_Mr_M[random_task-2].y*(PAGE_SIZE*2)*VIDEO_COLS + 
                         juego.posiciones_Mr_M[random_task-2].x*(PAGE_SIZE*2);
    paddr_t dir_dest = MAP_PHY_START + random_pos_y*(PAGE_SIZE*2)*VIDEO_COLS + random_pos_x*(PAGE_SIZE*2);
    //copia codigo
    move_code_Mr_M(dir_source, dir_dest);

    vaddr_t map_source = NEW_TASKS_VIRTUAL_START + slot_tarea*(PAGE_SIZE*2);
    mmu_unmap_page(juego.cr3_players[otro_jugador], map_source);
    mmu_unmap_page(juego.cr3_players[otro_jugador], map_source + PAGE_SIZE);
    mmu_map_page(juego.cr3_players[otro_jugador], map_source, dir_dest, 7);
    mmu_map_page(juego.cr3_players[otro_jugador], map_source + PAGE_SIZE, dir_dest + PAGE_SIZE, 7);
 
    juego.posiciones_Mr_M[random_task-2].x= random_pos_x;
    juego.posiciones_Mr_M[random_task-2].y= random_pos_y;
}