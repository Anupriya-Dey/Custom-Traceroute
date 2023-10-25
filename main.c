#include "traceroute.h"

int main(int argc, char **argv)
{
    t_traceroute trace;
    
    initialise_trace(&trace);
    debug(argc, argv);
    trace.ip = DNS_lookup(argv[1], &trace.destAddr);

    if (trace.ip)
    {
        printf("traceroute to %s (%s), 30 hops max,", argv[1], trace.ip);
        printf(" %d byte packets\n", (int)(sizeof(struct ip) + sizeof(struct icmphdr)));

        while (!(trace.hop == 31))
        {
            trace.count = 0;
            if (process_hop(&trace))
                break;
            trace.hop++;
        }
        free(trace.tempbuffer);
    }
    return (0);
}
