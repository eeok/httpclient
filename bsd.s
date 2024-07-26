global _start

extern ismain

_start:
    ; Access argc
    mov r9, [rdi]             ; r9 now contains argc
    mov rcx, r9	
    ; Access argv
    lea  rsi, [rdi+8]         ; rsi now points to argv[0]
    shl rcx, 3
    lea rdx, [rdi+8+rcx+8]
    ; Calculate the address of envp
    mov rdi, r9
    ;lea rdx, [rdx] ; rdx now points to envp

    ; Call the C function
    call ismain

    mov eax, 1                ; syscall number for exit
    xor edi, edi               ; exit code 0
    syscall
    ret


section .text
    global syswrite
    ; Arguments:
    ; rdi: file descriptor
    ; si: buffer pointer
    ; rdx: buffer size
syswrite:
    mov eax, 4
    syscall

    ret


section .text
    global sysread
sysread:
    ; Arguments:
    ; rdi: file descriptor
    ; rsi: buffer pointer
    ; rdx: buffer size

    mov eax, 3
    syscall

    ret


section .text
    global sysopen
sysopen:
    ; Arguments:
    ; rdi: pointer to filename
    ; rsi: flags
    ; rdx: mode
    mov eax, 5
    syscall

    ret

section .text
    global mkdr
mkdr:
    ; Arguments:
    ; rdi: pointer to dirname
    ; rsi: mode

    mov eax, 136 
    syscall

    ret

;---
section .text
    global sysclose
sysclose:
    mov eax, 6
    syscall
    ret

;---
section .text
    global seek
seek:
    mov eax, 478
    syscall 
    ret

;---
section .text
    global w8
w8:
    mov eax, 7
    syscall
    ret

;---
section .text
    global sysfork
sysfork:
    mov eax, 2
    syscall
    ret

;---
section .text
    global exec
exec:
    mov eax, 59
    syscall
    ret

;---
section .text
    global dread
dread:
    mov eax, 217
    syscall
    ret


;---
section .text
    global unlink
unlink:
    mov eax, 10
    syscall
    ret

