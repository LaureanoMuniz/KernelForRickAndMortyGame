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
juego_type juego;
void colocar_Megasemillas(){
    for(int i=0;i<CANT_MAX_MEGASEMILLAS;i++){
        bool esDistinto=false;
        while(!esDistinto){
            juego.posicion_Megasemillas[i].x=rand()%SIZE_N;
            juego.posicion_Megasemillas[i].y=rand()%SIZE_M;
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
    juego.cant_Megasemilla=CANT_MAX_MEGASEMILLAS;
    colocar_Megasemillas();
}