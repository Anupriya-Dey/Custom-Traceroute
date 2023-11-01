#include <traceroute.h>

void debug(int c, char **v)
{
	if (c == 1 || c >= 3)
		exit_err("usage: <outputFile> <hostname>\n");
	if (v[1][0] == '-' && v[1][2] == '\0')
		exit_err("usage: <outputFile> <hostname>\n");
}

void exit_err(char *s)
{
	printf("%s", s);
	exit(1);
}

char *dns_lookup(char *domain_name, struct sockaddr_in *connecAddr)
{
	struct addrinfo type; // it means we are looking for IPv4 address
	struct addrinfo *res;
	struct sockaddr_in *sa_in;
	char *ip;

	memset(&(type), 0, sizeof(type));
	type.ai_family = AF_INET;
	ip = malloc(INET_ADDRSTRLEN);
	if (getaddrinfo(domain_name, NULL, &type, &(res)) < 0)
		exit_err("unknown host\n");

	sa_in = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(res->ai_family, &(sa_in->sin_addr), ip, INET_ADDRSTRLEN);

	(*connecAddr) = *sa_in;
	(*connecAddr).sin_port = htons(1);
	return (ip);
}

void nmap(char* targetIP) {
    char nmapCommand[100];
    sprintf(nmapCommand, "nmap %s", targetIP);
 
    system(nmapCommand);
 
}
unsigned short checksum(char *buffer, int nwords)
{
	unsigned short *buf;
	unsigned long sum;

	buf = (unsigned short *)buffer;
	sum = 0;
	while (nwords > 0)
	{
		sum += *buf++;
		nwords--;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (~sum);
}

int process_hop(t_traceroute *p)
{
	while (p->i < 3)
	{
		p->sendbuff = create_packet(p->hopNo, p->ip, p->buffer);
		gettimeofday(&p->sendTime, NULL);
		sendto(p->sockfd, p->sendbuff, sizeof(struct ip) + sizeof(struct icmphdr),0, SA & p->destAddr, sizeof(p->destAddr));
		
		if (!(recvfrom(p->sockfd, p->recvbuff, sizeof(p->recvbuff), 0, SA & p->hopAddr, &p->len) <= 0))
		{
			gettimeofday(&p->recvTime, NULL);
			// calculation of RTT
			p->RTT = (double)((p->recvTime.tv_sec - p->sendTime.tv_sec) * 1000 + (p->recvTime.tv_usec - p->sendTime.tv_usec) / 1000.0);

			// calculation of bandwidth
			p->bandwidth = (double)((sizeof(p->sendbuff)) * 2) / p->RTT;

			p->icmph = (struct icmphdr *)(p->recvbuff + sizeof(struct ip));
			if ((p->icmph->type != 0))
			{
				// printf("%d",p->icmph->type);
				display_results(1, p, p->i);
			}
			else{
				display_results(3, p, p->i);
				if (p->i == 2)
				{
					return (1);
				}
			}
		}
		else display_results(2, p, p->i);
		p->i++;
	}
	return (0);
}
