#include <traceroute.h>

void *create_packet(int hopNo, char *ip, char *buff)
{
	struct ip *ip_hdr;
	struct icmphdr *icmphd;

	ip_hdr = (struct ip *)buff;
	ip_hdr->ip_hl = 5;
	ip_hdr->ip_v = 4;
	ip_hdr->ip_tos = 0;
	ip_hdr->ip_len = sizeof(struct ip) + sizeof(struct icmphdr);
	ip_hdr->ip_id = 10000;
	ip_hdr->ip_off = 0;
	ip_hdr->ip_ttl = hopNo;
	ip_hdr->ip_p = IPPROTO_ICMP;
	inet_pton(AF_INET, ip, &(ip_hdr->ip_dst));
	ip_hdr->ip_sum = checksum(buff, 9);

	icmphd = (struct icmphdr *)(buff + sizeof(struct ip));
	icmphd->type = ICMP_ECHO;
	icmphd->code = 0;
	icmphd->checksum = 0;
	icmphd->un.echo.id = 0;
	icmphd->un.echo.sequence = hopNo + 1;
	icmphd->checksum = checksum((buff + 20), 4);

	return (buff);
}

void initialise_trace(t_traceroute *trace)
{
	int one;
	int *val;

	one = 1;
	val = &one;
	trace->hopNo = 1;
	trace->tv_out.tv_sec = 1;
	trace->tv_out.tv_usec = 0;
	trace->len = sizeof(struct sockaddr_in);
	trace->buffer = malloc(4096);
	trace->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (setsockopt(trace->sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
		exit_err("error setsockopt\n");

	if (setsockopt(trace->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&trace->tv_out, sizeof(trace->tv_out)) < 0)
		exit_err("error setsockopt\n");
}

void display_results(int type, t_traceroute *p, int count)
{
	struct ip *ip;
	struct hostent *address;
	char *ipa;
	ipa = inet_ntoa(p->hopAddr.sin_addr);
	ip = (struct ip *)p->recvbuff;
	address = gethostbyaddr((void *)&(ip->ip_src.s_addr), sizeof(ip->ip_src.s_addr), AF_INET);

	if (type != 2)
	{
		if (count == 0)
		{
			printf("%d.) ", p->hopNo);
		}

		if (p->fail == count)
		{
			printf("IP Address: %s (%s) \n", address ? address->h_name : ipa, ipa);
		}

		printf("RTT: %.3f ms \n", p->RTT);
		if (count == 2) find_geolocation(ipa);
		if (count == 2)	printf("Estimated Bandwidth: %.2f bytes/ms\n\n", p->bandwidth);
		if (count == 2 && type == 3) nmap_port_discovery(ipa);

	}
	else
	{
		p->fail++;
		if (count == 0)
			printf("%d.)  * ", p->hopNo);
		else
			printf("*%s", (count == 2) ? "\n\n" : " ");
	}
}

void trace_network(t_traceroute *p)
{
	while (!(p->hopNo == 31))
	{
		p->i = 0;
		p->fail = 0;
		if (process_hop(p))
			break;
		p->hopNo++;
	}
}

int main(int argc, char **argv)
{
	t_traceroute trace;

	initialise_trace(&trace);
	debug(argc, argv);
	trace.ip = dns_lookup(argv[1], &trace.destAddr);

	if (trace.ip)
	{
		printf("traceroute to %s (%s), 30 hops max,", argv[1], trace.ip);
		printf(" %d byte packets\n", (int)(sizeof(struct ip) + sizeof(struct icmphdr)));
		trace_network(&trace);
		free(trace.buffer);
	}
	return (0);
}
