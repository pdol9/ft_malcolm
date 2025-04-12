#include "ft_malcolm.h"

/* get hostnames of source and target hosts */

void	hostname_info(char target_ip[IPV4_STR], char source_ip[IPV4_STR])
{
	struct in_addr addr;
	struct hostent *he;
	inet_aton(source_ip, &addr);
	he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if (he == NULL) {
		fprintf(stderr, "Failed to retrieve hostname of the source with"
				" provided IP address.");
	} else {
		printf("\tHostname of the source: %s", he->h_name);
	}
	printf("\n\tTarget IP: %s\n", target_ip);
	inet_aton(target_ip, &addr);
	he = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if (he == NULL) {
		fprintf(stderr, "Failed to retrieve hostname of the target.\n");
	} else {
		printf("\tHostname of the target: %s\n", he->h_name);
	}
}
