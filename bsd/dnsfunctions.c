#include "dnsfunctions.h"

#define BUF_SIZE 4096

struct iov {
 void *base;
 unsigned long len;
};

struct msg {
	void *msgname;
	unsigned int msglen;
	struct iov *iov;
	unsigned int iovlen;
	void *msgcontrol;
	unsigned int ctrllen;
	int flags;
};


long snd(int fd, const struct msg *m, int flags); 
long rcv(int fd, const struct msg *m, int flags); 
int sysconnect(int fd, struct msockaddr_in *ss, long namellen);
int sysgetpid();
int sock(int, int, int);
int sysclose(int fd);


min_addr_t ngethostbyname(unsigned char *host,unsigned char *dns_server) {
  unsigned char buf[4096],*qname,*reader;
  int i , j , stop , s;
  struct msockaddr_in a;
  struct RES_RECORD answers; //the replies from the DNS server
  struct msockaddr_in dest;
  int n;
  struct DNS_HEADER *dns = 0;
  struct QUESTION *qinfo = 0;
  struct msg msg;
  struct iov iov[1];


  s = sock( 2 , 2, 0); //UDP packet for DNS queries

  dest.sin_family = 2;
  dest.sin_port = my_htons(53);
  dest.sin_addr.s_addr = my_inet_addr(dns_server); //dns server
int bob;

	if((bob = sysconnect(s, &dest, sizeof(dest)))) {
		return -1;
	}

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
    n = rcv(s, &msg, 0);
    if (n < 0) {
        sysclose(s);
        return -1;
    }


  //move ahead of the dns header and the query field
  reader = &buf[sizeof(struct DNS_HEADER) + (len((const char*)qname)+1) + sizeof(struct QUESTION)];

unsigned char cool[256], namecool[256];
  //Start reading answers
  stop=0;

    answers.name= namecool;
    ReadName(reader,buf,&stop, answers.name);
    reader = reader + stop;

    answers.resource = (struct R_DATA*)(reader);
    reader = reader + sizeof(struct R_DATA);

      answers.rdata = cool; 

      for(j=0 ; j<my_ntohs(answers.resource->data_len) ; j++)
      {
        answers.rdata[j]=reader[j];
      }

      answers.rdata[my_ntohs(answers.resource->data_len)] = '\0';

      reader = reader + my_ntohs(answers.resource->data_len);


  //print answers

    if( my_ntohs(answers.resource->type) == T_A) //IPv4 address
    {
      long *p;
      p=(long*)answers.rdata;
      a.sin_addr.s_addr=(*p); //working without ntohl
        syswrite(1,custom_inet_ntoa(a.sin_addr), len(custom_inet_ntoa(a.sin_addr)));
    }

      syswrite(1, "\n", 1);


  return a.sin_addr.s_addr;
}

void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) {
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
