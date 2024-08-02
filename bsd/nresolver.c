#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "dnsfunctions.h"

int main(int argc, char const *argv[])
{
  unsigned char hostname[100];
char dns_server[20];
  if(argc < 3) {
    fprintf(stderr, "Usage:  %s <DNS Server> <Hostname>" 
        "<Iterative(0) / Recursive(1)>\n", argv[0]);
    return 1;
  }
  strncpy(dns_server,argv[1],20);
  strncpy(hostname,argv[2],100);
  ngethostbyname(hostname,dns_server);
    return 0;
}
