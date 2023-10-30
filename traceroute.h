/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 13:28:53 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/10/10 17:13:01 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#define SA (struct sockaddr *)
#define RECV_TIMEOUT 1

typedef struct s_traceroute
{
	char *buffer;
	char recvbuff[4096];
	socklen_t len;
	struct sockaddr_in destAddr;
	struct sockaddr_in hopAddr;
	struct icmphdr *icmph;
	char *sendbuff;
	char *ip;
	int hopNo;
	int sockfd;
	struct timeval tv_out;
	struct timeval sendTime;
	struct timeval recvTime;
	double RTT;
	int i;
} t_traceroute;

char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con);
unsigned short checksum(char *buffer, int nwords);
void exit_err(char *s);
void debug(int c, char **v);
int process_hop(t_traceroute *p);
void display_results(int type, t_traceroute *p, int n);
void *create_packet(int hopNo, char *ip, char *buff);

#endif
