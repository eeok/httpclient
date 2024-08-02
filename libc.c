#include "lib.h"

#define BUFFER_SIZE 1024
dword len(const char *str) {
    dword length = 0;
    
    // Iterate through the string until the null terminator is found
    while (*str != '\0') {
        length++;
        str++;
    }
    
    return length;
}
void zero(void *dest, unsigned long count) {
    unsigned char *ptr = (unsigned char *)dest;
    
    for (int i = 0; i < count; i++) {
        ptr[i] = 0;
    }
}

void copy(void *restrict dst, const void *restrict sc, long len) {
    char* src= (char*) sc;

    char* dest = (char*) dst;
    while(len--) {
	    *dest++ = *src++;
    }
}

void memset() {

}
