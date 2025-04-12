#include "ft_malcolm.h"

/* get IP address of the interface(s) */

static
void	ifc_addr_info(t_addr *addr_data)
{
	struct ifaddrs *ifaddr;
	char ip[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		fprintf(stderr, "\nFailed at retrieving IP address of source interfaces.\n"
				"Exiting ...\n");
		return ;
	}
	for (struct ifaddrs *ifa = ifaddr; ifa != NULL;	ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == AF_INET)
		{
			struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;

			if (inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN) == NULL) {
				fprintf(stderr, "\nFailed to retrieve IP address"
						" of source host.\nExiting ...\n");
				return ;
			}

			// Skip loopback address
			char *localhost = "127.0.0.1";
			if (ft_strncmp(ip, localhost, ft_strlen(localhost)) != 0) {
				printf("\tInterface: %s -> IP address: %s\n", ifa->ifa_name, ip);
				break;
			}
		}
	}
	print_MAC_addr("\tMAC address of source host: ", ':', addr_data->ifc_mac);
	printf("\n");
	freeifaddrs(ifaddr);

#if VERBOSE == 1
	hostname_info(addr_data->ipv4_name[1], ip);
#endif
}

int	retrieve_interface_info(t_addr *addr_data)
{
	struct ifaddrs *ifaddr, *active_ifc;
	struct rtnl_link_stats *stats;
	uint32_t max_tx_bytes = 0;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (ERROR);
	}

	printf("Retrieving host's interfaces' info ...\n");
	for (struct ifaddrs *ifa = ifaddr; ifa != NULL;	ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == AF_PACKET && ifa->ifa_data != NULL)
		{
			stats = ifa->ifa_data;

#if VERBOSE == 1
			// available interfaces:
			printf("* Found interface: %-8s \n", ifa->ifa_name);
			printf("\ttx_packets = %10u;   rx_packets = %10u\n"
					"\ttx_bytes   = %10u;   rx_bytes   = %10u\n"
					"\ttx_dropped = %10u;   rx_dropped = %10u\n"
					"\tmulticast  = %10u;   collisions = %10u\n\n",
					stats->tx_packets, stats->rx_packets,
					stats->tx_bytes, stats->rx_bytes,
					stats->tx_dropped, stats->rx_dropped,
					stats->multicast, stats->collisions);
#endif

			if (max_tx_bytes < stats->rx_bytes)
			{
				max_tx_bytes = stats->tx_bytes;
				active_ifc = ifa;
			}
		}
	}
	printf("Found available interface: %-8s \n", active_ifc->ifa_name);

	int if_index = if_nametoindex(active_ifc->ifa_name);
	if (if_index == -1) {
		perror("Failed to retrieve interface index: ");
		return (ERROR);
	}

#if VERBOSE == 1
	printf("Success: Interface index retrieved: -> %d\n", if_index);
#endif

	/* get MAC address of the interface */
	struct sockaddr_ll *s = (struct sockaddr_ll*)active_ifc->ifa_addr;
	for (int i = 0; i < s->sll_halen; i++)
		addr_data->ifc_mac[i] = s->sll_addr[i];

	freeifaddrs(ifaddr);
	ifc_addr_info(addr_data);

	return (if_index);
}
