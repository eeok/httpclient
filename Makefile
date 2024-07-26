UNAME := $(shell uname)


# Define the assembly file based on the operating system
ifeq ($(UNAME), Linux)
    ASM_FILE = linux64.s
else
    ASM_FILE = bsd.s
endif 


all: rest libc.o asm.o
	ld *.o -o http
rest:
	gcc  httpclient.c -c -O3 -fno-stack-protector -march=native

libc.o:
	gcc libc.c resolver.c -c -O1 -fno-stack-protector -march=native

asm.o:
	nasm -f elf64 linux64.s

clean:
	rm *.o
	rm http
