#include "traceroute.h"

int main(int argc, char** argv) {

    debug(argc, argv);
    char* ip;
    ip = DNS_lookup(argv[1]);
    printf("ip of domain %s\n",ip);
    
}

