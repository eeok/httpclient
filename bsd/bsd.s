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







