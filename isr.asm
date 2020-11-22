; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

sched_task_offset:     dd 0x00000000
sched_task_selector:   dw 0xFFFF  ;; Selector de Tarea Idle


global _isr32
global _isr33
global _isr88
global _isr89
global _isr100
global _isr123
;; PIC
extern pic_finish1

;; Sched
extern sched_next_task
extern print_exception
extern print_digito
extern sched_desalojar
extern change_state_debug
extern imprimir_debug
extern copiar_pantalla
extern servicio_meeseks
%define GDT_SEL_TSS_IDLE   21 << 3
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
    pushad      ;pusheo registros generales    
    mov ax, ds 
    push eax
    mov ax, es
    push eax
    mov ax, fs
    push eax
    mov ax, gs
    push eax
    mov eax, %1
    push eax
    call copiar_pantalla
    call imprimir_debug ;Se rompe no hace falta arreglar pila?
    ;call print_exception
    call sched_desalojar
    
%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20



;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;



_isr32:
    pushad
    call pic_finish1 
    call next_clock
    xchg bx, bx 
    ;;Scheduler
    call sched_next_task
    str cx
    cmp ax, cx
    je .fin
        mov [sched_task_selector], ax
        jmp far [sched_task_offset]
    .fin:
    popad
    iret 
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
_isr33:
    pushad
    in al, 0x60
    
    push eax
    call print_digito
    pop eax
    
    cmp al, 0x15
    jne .seguir 
        call change_state_debug
    .seguir:
    
    
    call pic_finish1
    popad
    iret
    
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

;uint32_t servicio_meeseks(uint32_t code, uint32_t x, uint32_t y)
_isr88:
    push edx
    push esp
    push ebp
    push esi
    push edi

    ;mov eax, 0x58
    push ecx
    push ebx
    push eax
    call servicio_meeseks
    add esp, 12
    jmp GDT_SEL_TSS_IDLE:0

    pop edi
    pop esi
    pop ebp
    pop esp
    pop edx
    iret 
_isr89:
    pushad
    mov eax, 0x59
    popad
    jmp GDT_SEL_TSS_IDLE:0
    iret 

_isr100:
    pushad
    mov eax, 0x64 
    popad
    jmp GDT_SEL_TSS_IDLE:0
    iret 

_isr123:
    pushad
    mov eax, 0x7b
    popad
    jmp GDT_SEL_TSS_IDLE:0
    iret 

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret

global saltar_idle

saltar_idle:
    jmp GDT_SEL_TSS_IDLE:0