/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "screen.h"

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
  initFila = 0;
  initCol  = 0;
  sizeFila = 40;
  sizeCol  = 80;
  character = ' ';
  attr = C_BG_LIGHT_GREEN;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  //Agrego barra roja
  initFila = 42;
  initCol  = 5;
  sizeFila = 3;
  sizeCol  = 10;
  character = ' ';
  attr = C_BG_LIGHT_RED;
  screen_draw_box(initFila, initCol, sizeFila, sizeCol, character, attr);
  
  //Agrego barra azul
  initFila = 42;
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
  uint32_t y = 43;
  uint16_t atr = C_FG_WHITE;
  print_hex(numero, size, x, y, atr);
  numero = 0;
  size = 8;
  x = 66;
  y = 43;
  atr = C_FG_WHITE;
  print_hex(numero, size, x, y, atr);              
}

void print_exception(uint32_t excepcion){
  print("EXCEPCION ",30,25,C_FG_WHITE|C_BG_GREEN);
  uint32_t tam=1+(excepcion>=10);
  print_dec(excepcion,tam,40,25,C_FG_WHITE|C_BG_GREEN);
}

