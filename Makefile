
all: rest libc.o asm.o
	ld *.o -o http
rest:
	gcc  httpclient.c -c -O3

libc.o:
	gcc libc.c resolver.c -c -O1

asm.o:
	nasm -f elf64 linux64.s

clean:
	rm *.o http