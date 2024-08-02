#include "resolver.h"

#define BUF_SIZE 4096

// Custom ntohs implementation
word custom_ntohs(word netshort) {
    return (netshort >> 8) | (netshort << 8);
}

// Function to convert a 16-bit integer from host to network byte order
word my_htons(word hostshort) {
    // Check the endianness of the system
    // If the system is little-endian, we need to swap the bytes
    word result;
    unsigned char *p = (unsigned char *)&result;

    // Check system endianness
    if (*(unsigned char *)&hostshort == (hostshort & 0xFF)) {
        // Little-endian: swap bytes
        p[0] = (hostshort >> 8) & 0xFF;
        p[1] = hostshort & 0xFF;
    } else {
        // Already big-endian: no need to swap
        result = hostshort;
    }
    
    return result;
}


void ReadName(unsigned char* reader,unsigned char* buffer,int* count, unsigned char* name)
{
  unsigned int p=0,jumped=0,offset;
  int i , j;

  *count = 1;

  //read the names in 3www6google3com format
  while(*reader!=0) {
    if(*reader>=192) {
      offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
      reader = buffer + offset - 1;
      jumped = 1; //we have jumped to another location so counting wont go up!
    }
    else {
      name[p++]=*reader;
    }
    reader = reader+1;
    if(jumped==0) {
      *count = *count + 1; //if we havent jumped to another location then we can count up
    }
  }
  name[p]='\0'; //string complete
  if(jumped==1) {
    *count = *count + 1; //number of steps we actually moved forward in the packet
  }
  //now convert 3www6google3com0 to www.google.com
  for(i=0;i<(int)len((const char*)name);i++) {
    p=name[i];
    for(j=0;j<(int)p;j++) {
      name[i]=name[i+1];
      i=i+1;
    }
    name[i]='.';
  }
  name[i-1]='\0'; //remove the last dot
}
// Helper function to convert a single byte to a string
void byte_to_str(unsigned char byte, char *str) {
    if (byte >= 100) {
        str[0] = '0' + byte / 100;
        byte %= 100;
        str[1] = '0' + byte / 10;
        byte %= 10;
        str[2] = '0' + byte;
        str[3] = '\0';
    } else if (byte >= 10) {
        str[0] = '0' + byte / 10;
        byte %= 10;
        str[1] = '0' + byte;
        str[2] = '\0';
    } else {
        str[0] = '0' + byte;
        str[1] = '\0';
    }
}

char* custom_inet_ntoa(struct min_addr in) {
    static char buffer[16];  // 16 bytes to hold "255.255.255.255\0"
    unsigned char *bytes = (unsigned char *)&in.s_addr;

    char byte_str[4];
    char *ptr = buffer;

    // Convert each byte to string and append to buffer
    for (int i = 0; i < 4; ++i) {
        byte_to_str(bytes[i], byte_str);
        for (char *p = byte_str; *p != '\0'; ++p) {
            *ptr++ = *p;
        }
        if (i < 3) {
            *ptr++ = '.';
        }
    }
    *ptr = '\0';

    return buffer;
}


dword my_inet_addr(const char *cp) {
    unsigned int b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    unsigned int *octets[] = {&b1, &b2, &b3, &b4};
    int current_octet = 0;
    
    // Loop through each character in the string
    while (*cp) {
        if (ISDIGIT(*cp)) {
            // Accumulate the digit
            *octets[current_octet] = *octets[current_octet] * 10 + (*cp - '0');
            
            // Check for overflow
            if (*octets[current_octet] > 255) {
                return mINADDR_NONE;
            }
        } else if (*cp == '.') {
            // Move to the next octet
            if (++current_octet > 3) {
                return mINADDR_NONE; // Too many octets
            }
        } else {
            return mINADDR_NONE; // Invalid character
        }
        cp++;
    }

    // Check if we have exactly four octets
    if (current_octet != 3) {
        return mINADDR_NONE;
    }

    // Combine the octets into a single 32-bit value
    return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}
static void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) {
  int lock = 0 , i;
  copy(host+len(host), ".", 1);
  for(i = 0 ; i < len((char*)host) ; i++) {
    if(host[i]=='.') {
      *dns++ = i-lock;
      for(;lock<i;lock++) {
        *dns++=host[lock];
      }
      lock++; //or lock=i+1;
    }
  }
  *dns++='\0';
}

min_addr_t ngethostbyname(unsigned char *host,unsigned char *dns_server) {
  unsigned char buf[4096],*qname,*reader;
  int i , j , stop , s;
  struct msockaddr_in a;
  struct RES_RECORD answer; //the replies from the DNS server
  struct msockaddr_in dest;

  struct DNS_HEADER *dns = 0;
  struct QUESTION *qinfo = 0;

  #ifdef __FreeBSD__
  struct msg msg;
  struct iov iov[1];
  #endif

  s = sock(mAF_INET , mSOCK_DGRAM , 0); //UDP packet for DNS queries

  dest.sin_family = mAF_INET;
  dest.sin_port = my_htons(53);
  dest.sin_addr.s_addr = my_inet_addr(dns_server); //dns server

  //Set the DNS structure to standard queries
  dns = (struct DNS_HEADER *)&buf;

  dns->id = (unsigned short) my_htons(sysgetpid());
  dns->qr = 0; //This is a query
  dns->opcode = 0; //This is a standard query
  dns->aa = 0; //Not Authoritative
  dns->tc = 0; //This message is not truncated
  dns->rd = 1; //Recursion Desired
  dns->ra = 0;
  dns->z = 0;
  dns->ad = 0;
  dns->cd = 0;
  dns->rcode = 0;
  dns->q_count = my_htons(1); //we have only 1 question
  dns->ans_count = 0;
  dns->auth_count = 0;
  dns->add_count = 0;

  //point to the query portion
  qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
  ChangetoDnsNameFormat(qname , host);
  qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) 
    + (len((const char*)qname) + 1)]; //fill it

  qinfo->qtype = my_htons( T_A ); // sending query of A record type
  qinfo->qclass = my_htons(1); //its internet

#ifdef __linux__
    // Connect to the DNS server
    if (sysconnect(s, &dest, sizeof(dest))) {
        syswrite(1, "Connect failed\n", 15);
        return  -1;
    }
       // Send DNS query
    syswrite(s, buf, sizeof(struct DNS_HEADER) + (len((const char*)qname) + 1) + sizeof(struct QUESTION));

       // Receive DNS response
    long received_len = sysread(s, buf, sizeof(buf));
    if (received_len < 0) {
        syswrite(1, "Read failed\n", 12);
        return -1;
    }
#else
  syswrite(1, "bsd\n", 4);
  // Prepare the message header for sending
    iov[0].base = buf;
    iov[0].len = sizeof(struct DNS_HEADER) + (len((const char*)qname) + 1) + sizeof(struct QUESTION);
    msg.msgname = 0;
    msg.msglen = 0;
    msg.iov = iov;
    msg.iovlen = 1;
    msg.msgcontrol = 0;
    msg.ctrllen = 0;
    msg.flags = 0;

    // Send DNS query
    if (snd(s, &msg, 0) < 0) {
        sysclose(s);
        return -1;
    }
  


    // Prepare the message header for receiving
    iov[0].base = buf;
    iov[0].len = BUF_SIZE;
    msg.msgname = 0;
    msg.msglen = 0;
    msg.iov = iov;
    msg.iovlen = 1;
    msg.msgcontrol = 0;
    msg.ctrllen = 0;
    msg.flags = 0;

    // Receive DNS response
    long n = rcv(s, &msg, 0);
    if (n < 0) {
        sysclose(s);
        return -1;
    }

#endif

  //move ahead of the dns header and the query field
  unsigned char cool[256];


  //Start reading answers
  reader = &buf[sizeof(struct DNS_HEADER) + (len((const char*)qname)+1) + sizeof(struct QUESTION)];
  stop=0;
  unsigned char namecool[256];
  answer.name = namecool;
  ReadName(reader,buf,&stop, answer.name);
  reader = reader + stop;

  answer.resource = (struct R_DATA*)(reader);
  reader = reader + sizeof(struct R_DATA);


  answer.rdata = cool;

  for(j=0 ; j<custom_ntohs(answer.resource->data_len) ; j++)
  {
    answer.rdata[j]=reader[j];
  }

  answer.rdata[custom_ntohs(answer.resource->data_len)] = '\0';

  reader = reader + custom_ntohs(answer.resource->data_len);
  

  //print answers
    if( custom_ntohs(answer.resource->type) == T_A) //IPv4 address
    {
      long *p;
      p=(long*)answer.rdata;
      a.sin_addr.s_addr=(*p); //working without ntohl
      syswrite(1, custom_inet_ntoa(a.sin_addr), len(custom_inet_ntoa(a.sin_addr)));
      syswrite(1, "\n", 1);
    }
    sysclose(s);
    return a.sin_addr.s_addr;
}



