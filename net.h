#ifndef NET_H
#define NET_H

#include "stdlib.h"


#define SOCK_STREAM 1


#define mAF_INET 2
#define mSOCK_DGRAM 2
#define mSOCK_STREAM 1
#define mIPPROTO_UDP 22
#define mINET_ADDRSTRLEN 16 
#define mINADDR_NONE ((dword) -1)

word my_htons(word hostshort);

#endif