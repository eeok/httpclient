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


;---
section .text
    global sysclose
sysclose:
    mov eax, 6
    syscall
    ret

;---
section .text
    global sock
sock:
mov eax, 97
    syscall
    ret
;---
section .text
    global sysconnect
sysconnect:
    mov eax, 98
    syscall
    ret

section .text
    global sysgetpid
sysgetpid:
    mov eax, 20
    syscall
    ret


section .text
    global snd
snd:
    mov eax, 28 
    syscall
    ret

section .text
    global rcv 
rcv:
    mov eax, 27
    syscall
    ret
