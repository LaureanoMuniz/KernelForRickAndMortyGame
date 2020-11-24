/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "screen.h"
#include "i386.h"
#include "sched.h"
#include "game.h"

char* clock = "|/-\\";

void print_char(char caracter, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  p[y][x].c=caracter;
  p[y][x].a=attr;
}

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}

void inicializar_pantalla(){ // checkear si hace falta dejar el espacio en negro arriba
  
  //Limpio pantalla 
  uint32_t initFila = 0;
  uint32_t initCol  = 0;
  uint32_t sizeFila = 50;
  uint32_t sizeCol  = 80;
  uint8_t character = ' ';
  uint8_t attr = C_BG_BLACK;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  
  //Pinto fondo de verde
  initFila = 1;
  initCol  = 0;
  sizeFila = 40;
  sizeCol  = 80;
  character = ' ';
  attr = C_BG_LIGHT_GREEN;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  //Agrego barra roja
  initFila = 43;
  initCol  = 5;
  sizeFila = 3;
  sizeCol  = 10;
  character = ' ';
  attr = C_BG_LIGHT_RED;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  
  //Agrego barra azul
  initFila = 43;
  initCol  = 65;
  sizeFila = 3;
  sizeCol  = 10;
  character = ' ';
  attr = C_BG_LIGHT_BLUE;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  
  //Printeo 0x0000 en cada caja
  //void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               //uint16_t attr) 
  uint32_t numero = 0;
  uint32_t size = 8;
  uint32_t x = 6;
  uint32_t y = 44;
  uint16_t atr = C_FG_WHITE;
  print_hex(numero, size, x, y, atr);
  numero = 0;
  size = 8;
  x = 66;
  y = 44;
  atr = C_FG_WHITE;
  print_hex(numero, size, x, y, atr);

  print("R",17,42, C_FG_LIGHT_RED | C_BG_BLACK);
   //print(" ",17,44,C_FG_WHITE | C_BG_BLACK);

  print("M",62,42, C_FG_LIGHT_BLUE | C_BG_BLACK);
   //print(" ",62,44,C_FG_WHITE | C_BG_BLACK);


  // Printeo los meeseeks de Rick.
  for(uint32_t i = 0; i<10; i++){
    print_dec(i,2,21+i*4,42, C_FG_LIGHT_RED | C_BG_BLACK);
    //print(" ",21+i*4,43,C_FG_WHITE | C_BG_BLACK);
  }
  // Printeo los meeseeks de Morty
  for(uint32_t i = 0; i<10; i++){
    print_dec(i,2,21+i*4,45,C_FG_LIGHT_BLUE | C_BG_BLACK);
    //print(" ",21+i*4,46,C_FG_WHITE | C_BG_BLACK);
  }              
}

void print_exception(uint32_t excepcion){
  print("EXCEPCION ",21,3,C_FG_WHITE|C_BG_BLACK);
  uint32_t tam=1+(excepcion>=10);
  print_dec(excepcion,tam,31,3,C_FG_WHITE|C_BG_BLACK);
}

//void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) 
void printLU(){

  print("498/19",20,25,C_FG_WHITE|C_BG_GREEN);
  print("460/19",20,26,C_FG_WHITE|C_BG_GREEN);
  print("577/18",20,27,C_FG_WHITE|C_BG_GREEN);
}

void print_digito(uint8_t digito){
 uint8_t claves[10] = {0x0B,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
  //uint32_t claves[10] = {0,27,49,50,51,52,53,54,55,56};
  //print_dec(digito,1,25,25,C_FG_WHITE|C_BG_BLACK);
 
 for (uint32_t i = 0; i<10; i++){
  if (digito == claves[i]){
   print_dec(i,1,79,0,C_FG_WHITE|C_BG_BLACK);
  }
 }

}
//uint32_t err,uint32_t eip, uint8_t cs, uint32_t eflags, uint32_t esp, uint8_t ss
void imprimir_debug(uint32_t num_excepcion, uint8_t gs, uint8_t fs, uint8_t es, uint8_t ds, 
                    uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t esp_kernel, 
                    uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax){
  
  uint32_t err; 
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t esp;
  uint32_t ss;
  uint32_t* pila = (uint32_t*) esp_kernel;
  if(num_excepcion == 8 || num_excepcion == 10 || num_excepcion == 11 ||
     num_excepcion == 12 || num_excepcion == 13 || num_excepcion == 14 || num_excepcion == 17){ //error code
    err = pila[0];
    eip = pila[1];
    cs = pila[2];
    eflags = pila[3];
    esp = pila[4];
    ss = pila[5];
  }
  else{
    err = 0;
    eip = pila[0];
    cs = pila[1];
    eflags = pila[2];
    esp = pila[3];
    ss = pila[4];
  }
  debug_state=2; 
  //  Imprime bloque negro
  uint32_t initFila = 1;
  uint32_t initCol  = 20;
  uint32_t sizeFila = 40;
  uint32_t sizeCol  = 40;
  uint32_t character = ' ';
  uint32_t attr = C_BG_BLACK;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);

  // Imprimir error 
  print_exception(num_excepcion);
  uint32_t id_tarea = sched.last_task[sched.turno];
  print_hex(id_tarea, 3, 56, 3, C_FG_LIGHT_GREEN | C_BG_BLACK);

  // Imprimir los el nombre de los registros y su contenido.
  char* columna_izq[15] = {"eax","ebx","ecx", "edx","esi", "edi", "ebp", "esp",  "eip", "cs", "ds", "es", "fs", "gs", "ss"};
  uint32_t columna_izq2[15] = {eax, ebx, ecx, edx, esi, edi, ebp, esp, eip, cs, ds, es, fs, gs, ss};
  for (int i = 0; i < 15; i++){
    print_hex(columna_izq2[i], 8, 26, 7+i*2, C_FG_LIGHT_GREEN | C_BG_BLACK);

    if(i < 9){
    print(columna_izq[i], 22, 7+i*2, C_FG_WHITE| C_BG_BLACK);}
    else{
    print(columna_izq[i], 23, 7+i*2, C_FG_WHITE| C_BG_BLACK);
    }
  }
 //Imprimir flags y sus contenidos
  print("eflags", 22, 39, C_FG_WHITE| C_BG_BLACK);
  print_hex(eflags, 8, 29, 39, C_FG_LIGHT_GREEN | C_BG_BLACK);

//Cargar registros de control con rcrx()
  uint32_t cr0 = rcr0();
  uint32_t cr2 = rcr2();
  uint32_t cr3 = rcr3();
  uint32_t cr4 = rcr4();

//Imprimir registros de control y contenido
  char* columna_der[5] = {"cr0", "cr2", "cr3","cr4","err"};
  uint32_t columna_der2[5] = {cr0, cr2, cr3, cr4, err};
    for (int i = 0; i < 5; i++){
    print_hex(columna_der2[i], 8, 51, 8+i*2, C_FG_LIGHT_GREEN | C_BG_BLACK);
    print(columna_der[i], 47 , 8+i*2, C_FG_WHITE| C_BG_BLACK);
  }
 

// Imprimir stack y contenido 
  //uint32_t* stack = (uint32_t*)esp;
  print("stack", 40 , 20, C_FG_WHITE| C_BG_BLACK);
  uint32_t fila = 22;
  uint32_t stack = esp;
  for(int i=0;i<3;i++){
    if(stack+4<=max_esp_task[id_tarea]){
      uint32_t* stack_p = (uint32_t*) stack; 
      print_hex(stack_p[0], 8, 40, fila, C_FG_LIGHT_GREEN | C_BG_BLACK);
      stack+=4;
    }
    else{
      print_hex(0, 8, 40, fila, C_FG_LIGHT_GREEN | C_BG_BLACK);
    }
    fila+=2;
  }
  
  // Imprimir backtrace y contenido 
  fila = 30;
  stack=ebp;
  print("backtrace", 40 , 28, C_FG_WHITE| C_BG_BLACK);
  for(int i=0;i<3;i++){
    if(stack+8<=max_esp_task[id_tarea]){
      uint32_t* stack_p = (uint32_t*) stack; 
      print_hex(stack_p[1], 8, 40, fila, C_FG_LIGHT_GREEN | C_BG_BLACK);
      stack=stack_p[0];
    }
    else{
      print_hex(0, 8, 40, fila, C_FG_LIGHT_GREEN | C_BG_BLACK);
    }
    fila+=2;
  }
}
void copiar_pantalla(){
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  for(int i=0;i<50;i++){
    for(int j=0;j<80;j++){
      copia_de_pantalla[i][j].a=p[i][j].a;
      copia_de_pantalla[i][j].c=p[i][j].c;
    }
  }
}
void devolver_pantalla(){
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  for(int i=0;i<50;i++){
    for(int j=0;j<80;j++){
      p[i][j].a=copia_de_pantalla[i][j].a;
      p[i][j].c=copia_de_pantalla[i][j].c;
    }
  }

}

void actualizar_puntajes(){
  uint32_t numero = juego.puntajes[0];
  uint32_t size = 8;
  uint32_t x = 6;
  uint32_t y = 44;
  uint16_t atr = C_FG_WHITE;
  print_dec(numero, size, x, y, atr);
  numero = juego.puntajes[1];
  size = 8;
  x = 66;
  y = 44;
  atr = C_FG_WHITE;
  print_dec(numero, size, x, y, atr);      
}

void actualizar_pantalla(){
    //Pinto fondo de verde
    uint32_t initFila = 1;
    uint32_t initCol  = 0;
    uint32_t sizeFila = 40;
    uint32_t sizeCol  = 80;
    uint8_t character = ' ';
    uint8_t attr = C_BG_LIGHT_GREEN;
    screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
    //Coloco las semillas
    for(int i=0;i<CANT_MAX_MEGASEMILLAS;i++){
      if(juego.posicion_Megasemillas[i].x != (uint32_t)-1){ // Si la semilla esta la pongo en pantalla
        print("S",juego.posicion_Megasemillas[i].x, juego.posicion_Megasemillas[i].y+1, 0x2E);
      }
    }

    for(int i=0;i<CANT_MAX_MR_M;i++){
      if(sched.state[i+2] != TASK_DEAD){
        print("K", juego.posiciones_Mr_M[i].x, juego.posiciones_Mr_M[i].y+1, juego.color_players[i%2]);
      }
    }
    actualizar_puntajes();
    if(sched.state[0] == TASK_DEAD || sched.state[1] == TASK_DEAD || juego.cant_Megasemilla==0){ // si termino el juego
      print_ganador();
    }

   

    // Printeo los relojes de Rick.
    print_char(clock[sched.reloj[0]],17,44,C_FG_WHITE | C_BG_BLACK);
    for(uint32_t i = 0; i<10; i++){
      if(sched.state[2*i+2]==TASK_DEAD){
        print("X",21+i*4,43,C_FG_WHITE | C_BG_BLACK);
      }
      else{
        print_char(clock[sched.reloj[2*i+2]],21+i*4,43,C_FG_WHITE | C_BG_BLACK);
      }
     
    }
    // Printeo los relojes de Morty
    print_char(clock[sched.reloj[1]],62,44,C_FG_WHITE | C_BG_BLACK);
    for(uint32_t i = 0; i<10; i++){
      if(sched.state[2*i+3]==TASK_DEAD){
        print("X",21+i*4,46,C_FG_WHITE | C_BG_BLACK);
      }
      else{
        print_char(clock[sched.reloj[2*i+3]],21+i*4,46,C_FG_WHITE | C_BG_BLACK);
      }
    }    
}

void print_ganador(){
  if(sched.state[0]==TASK_DEAD){ // Rick fue desalojado
    print("Gano Morty", 35, 0, C_FG_BLUE | C_BG_BLACK );
    return;
  }
  if(sched.state[1] == TASK_DEAD){ // Morty fue desalojado
    print("Gano Rick", 35, 0, C_FG_RED | C_BG_BLACK );
    return;
  }
  //Ninguna tarea fue desalojada -> se asimilaron todas las megasemillas
  if(juego.puntajes[0]==juego.puntajes[1]){
    print("Empate", 35, 0, C_FG_WHITE | C_BG_BLACK );
  }
  else if(juego.puntajes[0]>juego.puntajes[1]){//Gano Rick
    print("Gano Rick", 35, 0, C_FG_RED | C_BG_BLACK );
  }
  else{
    print("Gano Morty", 35, 0, C_FG_BLUE | C_BG_BLACK );
  }
}