#include "libft.h"
#include "ft_malcolm.h"

int	retrieve_host_LAN_info(t_addr *addr_data);
void	display_LAN_info(int iface_count, struct iface_info *infos, char *ipv_type);
void	hostname_info(char source_ip[IPV4_LENGTH], const char *host);

/******************************************************************************/

/* return an index for a name match to update missing info (IPv4 / IPv6 / MAC);
 * else add new interface;
 */

static
int add_ifc(struct iface_info *infos, int *count, const char *ifc_name)
{
	for (int i = 0; i < *count; i++)
	{
		if (ft_strncmp(infos[i].ifc_name, ifc_name, ft_strlen(ifc_name)) == 0)
			return i;
	}
	ft_strlcpy(infos[*count].ifc_name, ifc_name, IFNAMSIZ);
	return (*count)++;
}

/* store MAC address when linked list contains a node with MAC info about interface */

static
void	mac_info(struct ifaddrs *ifa, struct iface_info *infos)
{
	struct sockaddr_ll *ifc_addr = (struct sockaddr_ll *)ifa->ifa_addr;
	uint8_t *ifc_mac_addr = (uint8_t *)&ifc_addr->sll_addr;

	for (unsigned int i = 0; i < ifc_addr->sll_halen; ++i)
		infos->mac[i] = ifc_mac_addr[i];
}

/* store data if linked list contains a node with IPv4 info about specific interface;
 * select active interface;
 */

static
void	add_ipv4_info(struct ifaddrs *ifa, struct iface_info *infos)
{
	struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
	if (inet_ntop(AF_INET, &addr->sin_addr, infos->ipv4, INET_ADDRSTRLEN) == NULL)
	{
		fprintf(stderr, "\nFailed to retrieve IP address.\n");
		return ;
	}
	if (!(ifa->ifa_flags & IFF_UP))
		infos->is_up = false;
	else
		infos->is_up = true;
	infos->has_ipv4 = true;
}

/* store data if linked list contains a node with IPv6 info about specific interface */

static
void	add_ipv6_info(struct ifaddrs *ifa, struct iface_info *infos)
{
	struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)ifa->ifa_addr;
	if (inet_ntop(AF_INET6, &addr6->sin6_addr, infos->ipv6, INET6_ADDRSTRLEN) == NULL)
	{
		fprintf(stderr, "\nFailed to retrieve IP address.\n");
		return ;
	}
	if (!(ifa->ifa_flags & IFF_UP))
		infos->is_up = false;
	else
		infos->is_up = true;
	infos->has_ipv6 = true;
}

/* find (active) interface which has the highest value of received bytes */

static
struct ifaddrs	*find_active_ifc(t_addr *addr_data, struct ifaddrs *ifa, unsigned int *max_rx_bytes, uint8_t idx)
{
	if (ifa->ifa_data == NULL)
		return (NULL);
	struct rtnl_link_stats *stats = ifa->ifa_data;
	if (*max_rx_bytes < stats->rx_bytes)
	{
		*max_rx_bytes = stats->rx_bytes;
		addr_data->idx_active_ifc = idx;
		return (ifa);
	}
	return (NULL);
}

/* verify that active interface is available;
 * if successful, return index/descriptor of interface;
 */

static
int	verify_ifc(struct ifaddrs *active_ifc)
{
	/* validating host's interface availability */
	if (active_ifc == NULL)
	{
		fprintf(stderr, "\nUnsuccessful at validating presence of active interface. Exiting ...\n");
		return (ERROR);
	}
	fprintf(stdout, "Found available interface: %-8s\n", active_ifc->ifa_name);

	int if_index = if_nametoindex(active_ifc->ifa_name);
	if (if_index == -1) {
		fprintf(stderr, "Failed to retrieve interface index. Exiting... \n");
		return (ERROR);
	}

#if DEBUG == 1
	fprintf(stdout, "Interface index retrieved: %d\n", if_index);
#endif

	return if_index;
}

int	retrieve_host_LAN_info(t_addr *addr_data)
{
	struct iface_info infos[MAX_IFACES];
	struct ifaddrs *ifaddr, *active_ifc = NULL;
	int iface_count = 0;
	unsigned int current_rx = 0;

	if (getifaddrs(&ifaddr) == -1)
	{
		fprintf(stderr, "\ngetifaddrs failed. Exiting ...\n");
		return (ERROR);
	}

#if VERBOSE == 1
	fprintf(stdout, "Retrieving interface address table information ...\n");
#endif
	/* traverse through linked list which contains nodes about interface info */
	for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		int idx = add_ifc(infos, &iface_count, ifa->ifa_name);
		int family = ifa->ifa_addr->sa_family;
		if (family == AF_PACKET)
		{
			mac_info(ifa, &infos[idx]);
			active_ifc = find_active_ifc(addr_data, ifa, &current_rx, (uint8_t)idx);
		}
		else if (family == AF_INET)
			add_ipv4_info(ifa, &infos[idx]);
		else if (family == AF_INET6)
			add_ipv6_info(ifa, &infos[idx]);
	}

#if VERBOSE == 1
	fprintf(stdout, "Displaying interface address table:\n");
	display_LAN_info(iface_count, infos, "IPv4");
	display_LAN_info(iface_count, infos, "IPv6");
#endif

	int if_index = verify_ifc(active_ifc);
	if (if_index == ERROR)
		return (ERROR);
	addr_data->socket_address.sll_ifindex = if_index;

#if VERBOSE == 1
	hostname_info(infos[addr_data->idx_active_ifc].ipv4, "attacker");
	hostname_info(addr_data->ipv4_name[1], "target");
	hostname_info(addr_data->ipv4_name[0], "spoofed target");
#endif

	freeifaddrs(ifaddr);
	return (0);
}
