#include "ft_malcolm.h"

int retrieve_interface(struct ifreq *ifr, int *ft_index)
{
	struct ifaddrs *ifaddr;
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (-1);
	}

	uint32_t max_rx_bytes = 0;
	struct ifaddrs *if_tmp;
	struct rtnl_link_stats *stats;

	for (struct ifaddrs *ifa = ifaddr; ifa != NULL;	ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == AF_PACKET && ifa->ifa_data != NULL) {
			stats = ifa->ifa_data;
			if (max_rx_bytes < stats->rx_bytes) {
				max_rx_bytes = stats->rx_bytes;
				if_tmp = ifa;
			}
		}
	}
	printf("Retrieving host's interfaces ...\n"
			"Found available interface: %-8s \n", if_tmp->ifa_name);
	printf("\ttx_packets = %10u;   rx_packets = %10u\n"
		"\ttx_bytes   = %10u;   rx_bytes   = %10u\n",
		stats->tx_packets, stats->rx_packets,
		stats->tx_bytes, stats->rx_bytes);
	ft_strlcpy(ifr->ifr_name, if_tmp->ifa_name, ft_strlen(if_tmp->ifa_name));
	*ft_index = if_nametoindex(if_tmp->ifa_name);

	freeifaddrs(ifaddr);
	return (0);
}