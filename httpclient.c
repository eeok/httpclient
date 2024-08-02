#include "net.h"
#include "resolver.h"

int ismain(int argc, char const *argv[])
{
  unsigned char hostname[100];
  if(argc <  2) {
    return 1;
  }
  int pipi= len(argv[1]);
  copy(hostname,argv[1],pipi);
  min_addr_t addr = ngethostbyname(hostname,"1.1.1.1");

  int sockfd = sock(mAF_INET, SOCK_STREAM, 0);

#ifdef __linux__
  struct msockaddr_in sockadd = {
    .sin_family = mAF_INET,
    .sin_port = my_htons(80),
    .sin_addr.s_addr = addr
  };
#else
  struct msockaddr_in sockadd = {
    .sin_len = sizeof(struct msockaddr_in),
    .sin_family = mAF_INET,
    .sin_port = my_htons(80),
    .sin_addr.s_addr = addr
  };
#endif
  if (sysconnect(sockfd, &sockadd, sizeof(sockadd))) {
      syswrite(1,"Connection failed\n", 18);
      return 1;
  }
    
  char message[200] = "GET / HTTP/1.1\r\nHost: ";//Connection: close\r\n\r\n";
  int curlen = len(message);
  int lh = len(hostname);
  char *caca = "\r\nConnection: close\r\n\r\n";
  int lc = len(caca);
  copy(message+curlen, hostname, lh);
  copy(message+curlen+lh, caca, lc);

  syswrite(sockfd, message, curlen+lh+lc);
  syswrite(1, message, curlen+lh+lc);
  char buffer[4096];

  while (1) {
    long bytes_received = sysread(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_received <= 0) {
        break;
    }
    buffer[bytes_received] = '\0'; // Null-terminate the buffer
    syswrite(1,buffer, bytes_received);
  }
  
  syswrite(1,"\n",1);
  sysclose(sockfd);
  return 0;
}

