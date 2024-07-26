global _start

extern ismain

_start:
    ; Access argc
    mov rdi, [rsp]             ; rdi now contains argc

    ; Access argv
    lea rsi, [rsp + 8]         ; rsi now points to argv[0]

    ; Calculate the address of envp
    mov rcx, rdi               ; copy argc to rcx
    shl rcx, 3                 ; multiply argc by 8 (size of each pointer)
    lea rdx, [rsp + 8 + rcx + 8] ; rdx now points to envp

    ; Call the C function
    call ismain

    mov eax, 60                ; syscall number for exit
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
    mov eax, 1
    syscall

    ret


section .text
    global sysread
sysread:
    ; Arguments:
    ; rdi: file descriptor
    ; rsi: buffer pointer
    ; rdx: buffer size

    mov eax, 0
    syscall

    ret

;---
section .text
    global sysclose
sysclose:
    ; Arguments:
    ; rdi: file descriptor
    mov eax, 3
    syscall
    ret

;---
section .text
    global sock
sock:
    mov eax, 41
    syscall
    ret
;---
section .text
    global sysconnect
sysconnect:
    mov eax, 42
    syscall
    ret

section .text
    global sysgetpid
sysgetpid:
    mov eax, 39
    syscall
    ret
