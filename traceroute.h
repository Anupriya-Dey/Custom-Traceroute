#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>

typedef struct t_traceroute
{
	char *recvbuff;
	char *sendbuff;
	socklen_t len;
	struct sockaddr_in destAddr;
	struct sockaddr_in hopAddr;
	struct icmphdr *icmpheader;
	char *ip;
	int hop;
	int sockfd;
	struct timeval timeout;
	struct timeval sendtime;
	struct timeval recvtime;
	double rtt;
	int count;
} t_traceroute;

void initialise_trace(t_traceroute *trace);
void debug(int c, char **v);
char *DNS_lookup(char *domain_name, struct sockaddr_in *addr);
unsigned short checksum(char *buffer, int nwords);
int process_hop(t_traceroute *trace);
void *create_packet(int hop, char *ip);
void display_hop_info(int type, t_traceroute *trace, int n);

#endif
