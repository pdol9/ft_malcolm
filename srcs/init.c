#include "libft.h"
#include "ft_malcolm.h"

void	verbose_header_info(t_addr *addr_data);

void	init_memory(t_addr *addr_data)
{
	/* clear memory where Ethernet / ARP data will be stored */
	ft_memset(addr_data->buffer, 0x0, BUF_SIZE);

	/* initialize socket */
	addr_data->socket_address.sll_family = AF_PACKET;
	addr_data->socket_address.sll_protocol = htons(ETH_P_ARP);
	addr_data->socket_address.sll_hatype = htons(ARPHRD_ETHER);
	addr_data->socket_address.sll_pkttype = (PACKET_BROADCAST);
	addr_data->socket_address.sll_halen = MAC_LENGTH;
	addr_data->socket_address.sll_addr[6] = 0x0;
	addr_data->socket_address.sll_addr[7] = 0x0;

	/* Set memory to send and receive Ethernet / ARP data over raw socket  */
	addr_data->eth_header = (struct ethhdr *)addr_data->buffer;
	addr_data->arp_header = (struct arp_header *)(addr_data->buffer + ETH_HEADER_LEN);
}

int	init_arp_reply(t_addr **frame_data)
{
	fprintf(stdout, "Start initialization of ETH / ARP header ...\n");
	t_addr *addr_data = *frame_data;

	/* Set ETHERNET reply header */

	ft_memcpy(addr_data->eth_header->h_dest, addr_data->mac_addr_target, MAC_LENGTH);
	ft_memcpy(addr_data->eth_header->h_source, addr_data->mac_addr_src, MAC_LENGTH);
	addr_data->eth_header->h_proto = htons(ETH_P_ARP);

	/* Set ARP reply header */

	ft_memcpy(addr_data->arp_header->target_mac, addr_data->mac_addr_target, MAC_LENGTH);
	ft_memcpy(addr_data->arp_header->sender_mac, addr_data->mac_addr_src, MAC_LENGTH);
	ft_memcpy(addr_data->socket_address.sll_addr, addr_data->mac_addr_src, MAC_LENGTH);
	addr_data->arp_header->hardware_type = htons(HW_TYPE);
	addr_data->arp_header->protocol_type = htons(ETH_P_IP);
	addr_data->arp_header->hardware_len = MAC_LENGTH;
	addr_data->arp_header->protocol_len = IPV4_LENGTH;
	addr_data->arp_header->opcode = htons(ARP_REPLY);

	// Set source and target IP
	for (int index = 0; index < 4; ++index)
	{
		addr_data->arp_header->sender_ip[index] = addr_data->ipv4_addr[0][index];
		addr_data->arp_header->target_ip[index] = addr_data->ipv4_addr[1][index];
	}

#if VERBOSE == 1
	fprintf(stdout, "Initialized data: \n");
	verbose_header_info(addr_data);
#endif

	return 0;
}
