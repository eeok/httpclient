#include "ndns.h"
#include "stdlib.h"

min_addr_t ngethostbyname(unsigned char *, unsigned char *);
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);
void ReadName (unsigned char*,unsigned char*,int*, unsigned char*);

unsigned short my_htons(unsigned short);
unsigned short my_ntohs(unsigned short);

unsigned int my_inet_addr(const char *);
char *custom_inet_ntoa(struct min_addr);
