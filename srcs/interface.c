#include "ft_malcolm.h"

inline void	print_MAC_addr(const char *str, uint8_t arr[MAC_LENGTH])
{
	printf("%s MAC address: ", str);
	int i = 0;
	for (; i < MAC_LENGTH - 1; ++i)
		printf("%02x%c", (arr[i]), ':');
	printf("%02x", (arr[i]));
}

int retrieve_interface_info(struct ifreq *ifr, int *ft_index, uint8_t ifc_mac[MAC_LENGTH])
{
	struct ifaddrs *ifaddr;
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (-1);
	}

	uint32_t max_rx_bytes = 0;
	struct ifaddrs *if_tmp;
	struct rtnl_link_stats *stats;

	printf("Retrieving host's interface info ...\n");
	for (struct ifaddrs *ifa = ifaddr; ifa != NULL;	ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == AF_PACKET && ifa->ifa_data != NULL)
		{
			stats = ifa->ifa_data;
			/*  --verbose  */
//			printf("* Found interface: %-8s \n", ifa->ifa_name);
//			printf("\ttx_packets = %10u;   rx_packets = %10u\n"
//					"\ttx_bytes   = %10u;   rx_bytes   = %10u\n"
//					"\ttx_dropped = %10u;   rx_dropped = %10u\n"
//					"\tmulticast  = %10u;   collisions = %10u\n\n",
//					stats->tx_packets, stats->rx_packets,
//					stats->tx_bytes, stats->rx_bytes,
//					stats->tx_dropped, stats->rx_dropped,
//					stats->multicast, stats->collisions);
			if (max_rx_bytes < stats->rx_bytes) {
				max_rx_bytes = stats->rx_bytes;
				if_tmp = ifa;
			}
		}
	}
	//printf("Found available interface: %-8s \n", if_tmp->ifa_name);
	//printf("\ttx_packets = %10u;   rx_packets = %10u\n"
	//	"\ttx_bytes   = %10u;   rx_bytes   = %10u\n"
	//	"\ttx_dropped = %10u;   rx_dropped = %10u\n"
	//	"\tmulticast  = %10u;   collisions = %10u\n----------------\n",
	//	stats->tx_packets, stats->rx_packets,
	//	stats->tx_bytes, stats->rx_bytes,
	//	stats->tx_dropped, stats->rx_dropped,
	//	stats->multicast, stats->collisions);

	ft_strlcpy(ifr->ifr_name, if_tmp->ifa_name, ft_strlen(if_tmp->ifa_name));
	*ft_index = if_nametoindex(if_tmp->ifa_name);
	struct sockaddr_ll *s = (struct sockaddr_ll*)if_tmp->ifa_addr;
	for (int i = 0; i < s->sll_halen; i++)
		ifc_mac[i] = s->sll_addr[i];

	/* print interface MAC address */
	//print_MAC_addr("Source interface", s->sll_addr);
	freeifaddrs(ifaddr);
	return (0);
}
