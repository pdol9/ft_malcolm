#include "ft_malcolm.h"

/******************************************************************************/

/* traverse through linked list which contains nodes with info about interfaces;
 * return index on a name match to update missing info (IPv4 / IPv6 / MAC);
 */

static
int add_or_update_ifc(struct iface_info *infos, int *count, const char *name)
{
	for (int i = 0; i < *count; i++)
	{
		if (ft_strncmp(infos[i].name, name, ft_strlen(name)) == 0)
			return i;
	}
	ft_strlcpy(infos[*count].name, name, IFNAMSIZ);
	return (*count)++;
}

/* when linked list contains a node with MAC info about interface;
 * store target's MAC address after validating it;
 */

static
void	mac_info(struct ifaddrs *ifa, struct ifaddrs **target_mac_ifc,
					struct iface_info *infos, t_addr *addr_data)
{
	struct sockaddr_ll *ifc_addr = (struct sockaddr_ll *)ifa->ifa_addr;

	uint8_t *ifc_mac_addr = (uint8_t *)&ifc_addr->sll_addr;
	for (unsigned int i = 0; i < ifc_addr->sll_halen; ++i)
		infos->mac[i] = ifc_mac_addr[i];

	// validation
	for (unsigned int i = 0; i < ifc_addr->sll_halen; ++i)
	{
		if (addr_data->mac_addr_target[i] != ifc_mac_addr[i])
			return ;
	}
	*target_mac_ifc = ifa;

	// store MAC address of the interface for initialization of arp / eth header
	for (int i = 0; i < ifc_addr->sll_halen; i++)
		addr_data->ifc_mac[i] = ifc_addr->sll_addr[i];
}

/* when linked list contains a node with IPv4 info about interface;
 * validate target's IP after retrieving info;
 */

static
void	ipv4_info(struct ifaddrs *ifa, struct ifaddrs **target_ip_ifc,
	       struct iface_info *infos, t_addr *addr_data)
{
	struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
	if (inet_ntop(AF_INET, &addr->sin_addr, infos->ipv4, INET_ADDRSTRLEN) == NULL)
	{
		fprintf(stderr, "\nFailed to retrieve IP address. Exiting ...\n");
		return ;
	}
	if (!(ifa->ifa_flags & IFF_UP))
		infos->is_up = false;
	else
		infos->is_up = true;
	infos->has_ipv4 = true;

	// validation
	uint8_t *ifc_ip_addr = (uint8_t *)&addr->sin_addr;
	uint8_t *target_ip_addr = addr_data->ipv4_addr[1];
	for (unsigned int i = 0; i < sizeof(struct in_addr); ++i)
	{
		if (target_ip_addr[i] != ifc_ip_addr[i])
			return ;
	}
	*target_ip_ifc = ifa;
}

/* when linked list contains a node with IPv6 info about interface;
 * store info;
 */

static
void	ipv6_info(struct ifaddrs *ifa, struct iface_info *infos)
{
	struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)ifa->ifa_addr;
	if (inet_ntop(AF_INET6, &addr6->sin6_addr, infos->ipv6, INET6_ADDRSTRLEN) == NULL)
	{
		fprintf(stderr, "\nFailed to retrieve IP address. Exiting ...\n");
		return ;
	}
	if (!(ifa->ifa_flags & IFF_UP))
		infos->is_up = false;
	else
		infos->is_up = true;
	infos->has_ipv6 = true;
}

/* verbose output of LAN info */

static
void	display_LAN_info(int iface_count, struct iface_info *infos, char *ipv_type)
{
	for (int i = 0; i < iface_count; ++i)
	{
		fprintf(stdout, "   Interface %-16s ", infos[i].name);
		if (infos[i].is_up == false)
			fprintf(stdout, "%-34s", "(IP):   N/A - status: [DOWN]");
		else if (ft_strncmp(ipv_type, "IPv4", 4) == 0 && infos[i].has_ipv4)
			fprintf(stdout, "(IPv4): %-26s", infos[i].ipv4);
		else if (ft_strncmp(ipv_type, "IPv6", 4) == 0 && infos[i].has_ipv6)
			fprintf(stdout, "(IPv6): %-26s", infos[i].ipv6);
		print_MAC_addr(" :: MAC: ", ':', infos[i].mac);
		fprintf(stdout, "\n");
	}
}

/* check that target's IP and MAC adresseses are present on LAN;
 * if successful, return index of interface;
 */

static
int	verify_ifc(struct ifaddrs *target_ip_ifc, struct ifaddrs *target_mac_ifc)
{
	// validating target IP and MAC address on available interfaces
	if (target_ip_ifc == NULL || target_mac_ifc == NULL)
	{
		fprintf(stderr, "\nUnsuccessful at validating presence of target IP address on LAN. Exiting ...\n");
		return (ERROR);
	}
	fprintf(stdout, "*** Found a match of target IP address on interface: %-8s ***\n", target_ip_ifc->ifa_name);

	int if_index = if_nametoindex(target_ip_ifc->ifa_name);
	if (if_index == -1) {
		perror("Failed to retrieve interface index: ");
		return (ERROR);
	}

#if VERBOSE == 1
	printf("Success: Interface index retrieved: %d\n", if_index);
#endif

	return if_index;
}

/// @brief get information of the local network
/// @param addr_data 
/// @return Interface index on success or ERROR

int	retrieve_LAN_info(t_addr *addr_data)
{
	struct iface_info infos[MAX_IFACES];
	struct ifaddrs *ifaddr, *target_ip_ifc = NULL, *target_mac_ifc = NULL;
	int iface_count = 0;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return (ERROR);
	}

	printf("Retrieving LAN information ...\n");
	for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		int idx = add_or_update_ifc(infos, &iface_count, ifa->ifa_name);
		int family = ifa->ifa_addr->sa_family;
		if (family == AF_PACKET)
			mac_info(ifa, &target_mac_ifc, &infos[idx], addr_data);
		else if (family == AF_INET)
			ipv4_info(ifa, &target_ip_ifc, &infos[idx], addr_data);
		else if (family == AF_INET6)
			ipv6_info(ifa, &infos[idx]);
	}

#if VERBOSE == 1
	fprintf(stdout, "Displaying LAN information:\n");
	display_LAN_info(iface_count, infos, "IPv4");
	display_LAN_info(iface_count, infos, "IPv6");
#endif

	int if_index = verify_ifc(target_ip_ifc, target_mac_ifc);

	freeifaddrs(ifaddr);
	return (if_index);
}