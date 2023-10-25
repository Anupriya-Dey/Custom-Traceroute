#include "traceroute.h"

void debug(int argc, char **argv)
{
    if (argc == 1 || argc >= 3)
    {
        printf("usage: ./main hostname\n");
        exit(1);
    }
    if (argv[1][0] == '-' && argv[1][2] == '\0')
    {
        printf("usage: ./main hostname\n");
        exit(1);
    }
}

void initialise_trace(t_traceroute *trace)
{
    int one = 1;
    int *val = &one;

    trace->hop = 1;
    trace->timeout.tv_sec = 1;
    trace->timeout.tv_usec = 0;
    trace->len = sizeof(struct sockaddr_in);
    trace->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (setsockopt(trace->sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
    {
        printf("error setsockopt\n");
        exit(1);
    }

    setsockopt(trace->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&trace->timeout, sizeof(trace->timeout));
}

char *DNS_lookup(char *domain_name, struct sockaddr_in *addr)
{
    struct addrinfo type; // it means we are looking for IPv4 address
    struct addrinfo *res_addr;
    struct sockaddr_in *sa_in;
    char *ip;

    memset(&(type), 0, sizeof(type));
    type.ai_family = AF_INET;
    ip = malloc(INET_ADDRSTRLEN); // value is 16
    if (getaddrinfo(domain_name, NULL, &type, &(res_addr)) < 0)
    {
        printf("unknown host\n");
        exit(1);
    }

    sa_in = (struct sockaddr_in *)res_addr->ai_addr;
    inet_ntop(res_addr->ai_family, &(sa_in->sin_addr), ip, INET_ADDRSTRLEN);
    (*addr) = *sa_in;
    (*addr).sin_port = htons(1);
    return (ip);
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
    sum = (sum >> 16) + (sum & 0xffff);
    return (~sum);
}

void *create_packet(int hopNo, char *ip)
{
    struct ip *ip_hdr;
    struct icmphdr *icmp_hdr;
    char *buff = malloc(4096);
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

    icmp_hdr = (struct icmphdr *)(buff + sizeof(struct ip));
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->checksum = 0;
    icmp_hdr->un.echo.id = 0;
    icmp_hdr->un.echo.sequence = hopNo + 1;
    icmp_hdr->checksum = checksum((buff + 20), 4);

    return (buff);
}

int process_hop(t_traceroute *trace)
{
    while (trace->count < 3)
    {
        trace->sendbuff = create_packet(trace->hop, trace->ip);
        gettimeofday(&trace->sendtime, NULL);

        sendto(trace->sockfd, trace->sendbuff, sizeof(struct ip) + sizeof(struct icmphdr),
               0, (struct sockaddr *)&trace->destAddr, sizeof(trace->destAddr));

        if (!(recvfrom(trace->sockfd, trace->recvbuff, sizeof(trace->recvbuff), 0,
                       (struct sockaddr *)&trace->hopAddr, &trace->len) <= 0))
        {
            gettimeofday(&trace->recvtime, NULL);

            trace->rtt = (double)((trace->recvtime.tv_sec - trace->sendtime.tv_sec) + (trace->recvtime.tv_usec - trace->sendtime.tv_usec) / 1000.0); // time taken start-end
            trace->icmpheader = (struct icmphdr *)(trace->recvbuff + sizeof(struct ip));
            
            
            //display info
        }
        else
            //display info

        trace->count++;
    }
    return (0);
}
