#include "ft_malcolm.h"

/* get hostname of local host */

void   hostname_info(char source_ip[IPV4_LENGTH])
{
       struct in_addr addr;
       struct hostent *he;
       inet_aton(source_ip, &addr);
       he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
       if (he == NULL) {
               fprintf(stderr, "Failed to retrieve hostname of the source with "
                               "provided IP address.\n");
       } else {
               fprintf(stdout, "   Hostname of attacker: %s\n", he->h_name);
       }
}
