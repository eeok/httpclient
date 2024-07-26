#ifndef MSTDLIB_H
#define MSTDLIB_H

#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')

typedef unsigned char           byte;
typedef unsigned long long      qword;
typedef unsigned int            dword;
typedef unsigned short          word;

int sock(int domain, int type, int protocol);
long long syswrite(int fd, const void *buf, unsigned long count);
long long sysread(int fd, const void *buf, unsigned long count);
int sysclose(int fd);
int sysgetpid();



#define	SOCKADDR_COMMON(sa_prefix) \
  unsigned short int sa_prefix##family


/* Internet address.  */
typedef dword min_addr_t;
struct min_addr
  {
    min_addr_t s_addr;
  };

  /* Structure describing a generic socket address.  */
struct msockaddr
  {
    SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };
#define SOCKADDR_COMMON_SIZE	(sizeof (unsigned short int))

/* Structure describing an Internet socket address.  */
struct msockaddr_in
  {
    SOCKADDR_COMMON (sin_);
    word sin_port;			/* Port number.  */
    struct min_addr sin_addr;		/* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct msockaddr)
			   - SOCKADDR_COMMON_SIZE
			   - sizeof (word)
			   - sizeof (struct min_addr)];
  };

int sysconnect(int sockfd, const struct msockaddr_in *addr, dword addrlen);

void copy(void * dst, const void * sc, long len);
dword len(const char *str);

#endif