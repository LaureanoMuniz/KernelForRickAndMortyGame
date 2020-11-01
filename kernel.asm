; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start
extern inicializar_pantalla
extern GDT_DESC 
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg


%define INIT_STACK 0x00025000
%define SPACE 32
%define TAMANO1 8000
%define TAMANO2 6400
%define GDT_OFF_C0_DESC   10 << 3
%define GDT_OFF_D0_DESC   11 << 3
%define GDT_OFF_C3_DESC   12 << 3
%define GDT_OFF_D3_DESC   13 << 3
%define GDT_OFF_VID_DESC  14 << 3

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    ; pruebo poner un breakpoint
    xchg bx,bx
    
    ; Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax,CR0
    or eax, 1
    mov CR0, eax
    ; Saltar a modo protegido
    jmp GDT_OFF_C0_DESC:modo_protegido

BITS 32
modo_protegido:

    ; Establecer selectores de segmentos
    mov ax, GDT_OFF_D0_DESC
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax ;esto anda, no? si

    ; Establecer la base de la pila
    mov ebp, INIT_STACK
    mov esp, INIT_STACK        ; esto está bien, no? si
    
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x0F, 25, 25

    ; Inicializar pantalla
    ;mov ax, GDT_OFF_VID_DESC  
    ;mov ds, ax                
    
    ;mov ecx, 0 ; contador 
    ;mov edx, 0 ; contador 

    ; Rutina para limpiar pantalla
    ;.limpiar:
        ;cmp ecx, TAMANO1
        ;je .pintar
        ;mov ax, 0x0020   ; fondo negro, letra negra, caracter espacio
        ;mov [ds:ecx], ax
        ;add ecx, 2
        ;jmp .limpiar
    ; Rutina para pintar pantalla    
    ;.pintar:
        ;cmp edx, TAMANO2
        ;je .fin
        ;mov ax, 0x5020 ; fondo magenta, letra negra, caracter espacio
        ;mov [ds:edx], ax
        ;add edx, 2
        ;jmp .pintar
    xchg bx,bx
    call inicializar_pantalla ;Fijarse si puedo usar un selector de segmento en C. No
    ; Inicializar el manejador de memoria
    
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler
    xchg bx, bx
    ; Inicializar la IDT
    call idt_init
    ; Cargar IDT
    lidt [IDT_DESC]
   
    xor eax, eax
    idiv eax
    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable
    
    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti 
    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
