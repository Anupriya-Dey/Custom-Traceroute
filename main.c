#include "traceroute.h"

int main(int argc, char **argv)
{
    t_traceroute trace;
    
    initialise_trace(&trace);
    debug(argc, argv);
    trace.ip = DNS_lookup(argv[1], &trace.destAddr);

    if (trace.ip)
    {
        while (!(trace.hop == 31))
        {
            trace.count = 0;
            if (process_hop(&trace))
                break;
            trace.hop++;
        }
    }
    return (0);
}
