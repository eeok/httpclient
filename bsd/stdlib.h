#ifndef MSTDLIB_H
#define MSTDLIB_H

#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')

typedef unsigned char           byte;
typedef unsigned long long      qword;
typedef unsigned int            dword;
typedef unsigned short          word;




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

struct msockaddr_in {
  byte sin_len;
  byte sin_family;
  word sin_port;
  struct min_addr sin_addr;
  char sin_zero[8];
};


 /* {
    SOCKADDR_COMMON (sin_);
    word sin_port;	
    struct min_addr sin_addr;

    unsigned char sin_zero[sizeof (struct msockaddr)
			   - SOCKADDR_COMMON_SIZE
			   - sizeof (word)
			   - sizeof (struct min_addr)];
  };
*/

void copy(void * dst, const void * sc, long len);
dword len(const char *str);

#endif
