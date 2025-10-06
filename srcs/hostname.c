#include "ft_malcolm.h"

/* get hostname of local host */

void   hostname_info(char source_ip[IPV4_LENGTH], const char *host)
{
       struct in_addr addr;
       struct hostent *he;

       inet_aton(source_ip, &addr);
       he = gethostbyaddr(&addr, sizeof(addr), AF_INET);

       if (he == NULL) fprintf(stderr, "Failed to retrieve hostname of %s with " \
                               "provided IP address.\n", host);
       else fprintf(stdout, "   Hostname of %s: %s\n", host, he->h_name);
}
