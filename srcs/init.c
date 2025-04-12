#include "ft_malcolm.h"

int	init_data(t_addr *addr_data)
{
	printf("Entering initialiation phase ...\n");
	int if_index = retrieve_interface_info(addr_data);
	if (if_index == -1)
		return (ERROR);

	/* Buffer to receive and store packet information */
	ft_memset(addr_data->buffer, 0x0, sizeof(addr_data->buffer));

	/* Set socket */
	addr_data->socket_address.sll_family = AF_PACKET;
	addr_data->socket_address.sll_protocol = htons(ETH_P_ARP);
	addr_data->socket_address.sll_ifindex = if_index;
	addr_data->socket_address.sll_hatype = htons(ARPHRD_ETHER);
	addr_data->socket_address.sll_pkttype = (PACKET_BROADCAST);
	addr_data->socket_address.sll_halen = MAC_LENGTH;
	addr_data->socket_address.sll_addr[6] = 0x0;
	addr_data->socket_address.sll_addr[7] = 0x0;

	/* Set memory to send from and receive Ethernet / ARP data over raw socket  */
	addr_data->eth_req = (struct ethhdr *)addr_data->buffer;
	addr_data->eth_reply = (struct ethhdr *)addr_data->buffer;
	addr_data->arp_req = (struct arp_header *)(addr_data->buffer + ETH2_HEADER_LEN);
	addr_data->arp_reply = (struct arp_header *)(addr_data->buffer + ETH2_HEADER_LEN);

	/* Set ETHERNET request header */

	ft_memset(addr_data->eth_req->h_dest, 0xff, MAC_LENGTH);
	ft_memcpy(addr_data->eth_req->h_source, addr_data->mac_addr_src, MAC_LENGTH);
	addr_data->eth_req->h_proto = htons(ETH_P_ARP);

	/* Set ARP request header */

	ft_memset(addr_data->arp_req->target_mac, 0x0, MAC_LENGTH);
	ft_memcpy(addr_data->arp_req->sender_mac, addr_data->mac_addr_src, MAC_LENGTH);
	ft_memcpy(addr_data->socket_address.sll_addr, addr_data->mac_addr_src, MAC_LENGTH);
	addr_data->arp_req->hardware_type = htons(HW_TYPE);
	addr_data->arp_req->protocol_type = htons(ETH_P_IP);
	addr_data->arp_req->hardware_len = MAC_LENGTH;
	addr_data->arp_req->protocol_len = IPV4_LENGTH;
	addr_data->arp_req->opcode = htons(ARP_REQUEST);

	// Set source and target IP
	for (int index = 0; index < 4; ++index)
	{
		addr_data->arp_req->sender_ip[index] = addr_data->ipv4_addr[0][index];
		addr_data->arp_req->target_ip[index] = addr_data->ipv4_addr[1][index];
	}

#if VERBOSE == 1
	verbose_req_package(addr_data);
#endif

	return 0;
}
