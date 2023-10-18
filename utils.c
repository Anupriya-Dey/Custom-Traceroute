#include "traceroute.h"

void debug(int argc, char **argv)
{
    if (argc == 1 || argc >= 3)
    {
        printf("usage: ./main [-h] hostname\n");
        exit(1);
    }
    if (argv[1][0] == '-' && argv[1][2] == '\0')
    {
        printf("usage: ./main [-h] hostname\n");
        exit(1);
    }
}

char *DNS_lookup(const char *domain_name)
{
    struct hostent *host_info;
    struct in_addr **address_list;
    int i;

    host_info = gethostbyname(domain_name);

    if (host_info == NULL)
    {
        fprintf(stderr, "DNS lookup failed for the given domain name\n");
        return NULL;
    }
    else
    {
        address_list = (struct in_addr **)host_info->h_addr_list;
        char *ip_address = (char *)malloc(INET_ADDRSTRLEN * sizeof(char)); // Allocate memory for IP address

        if (ip_address == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(ip_address, inet_ntoa(*address_list[0])); // Convert in_addr to string and copy it to allocated memory

        return ip_address;
    }
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
