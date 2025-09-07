#include "ft_malcolm.h"

/*
 *	print Ethernet / ARP packet info
 */

inline
void	verbose_header_info(t_addr *addr_data)
{
	uint16_t h_proto = ntohs(addr_data->eth_header->h_proto);
	uint16_t hw_type = ntohs(addr_data->arp_header->hardware_type);
	uint16_t protocol_type = ntohs(addr_data->arp_header->protocol_type);
	uint16_t opcode = ntohs(addr_data->arp_header->opcode);

	/*----------*----------*	ETH header	*----------*----------*/
	fprintf(stdout, "\n\t\tEthernet / ARP header\n"
			"     +--------+--------+--------+--------+--------+--------+\n");
	print_MAC_addr("\t", ' ', addr_data->eth_header->h_dest);
	print_MAC_addr("", ' ', addr_data->eth_header->h_source);
	fprintf(stdout, " %02x %02x %02x %02x\n\t %02x %02x %02x %02x %02x %02x",
		(h_proto >> 8) & 0xFF, h_proto & 0xFF,
	/*----------*----------*	ARP header	*----------*----------*/
		(hw_type >> 8) & 0xFF, hw_type & 0xFF,
		(protocol_type >> 8) & 0xFF, protocol_type & 0xFF,
		addr_data->arp_header->hardware_len,
		addr_data->arp_header->protocol_len,
		(opcode >> 8) & 0xFF, opcode & 0xFF);
	print_MAC_addr("", ' ', addr_data->arp_header->sender_mac);
	fprintf(stdout, " %s", inet_ntop(AF_INET, addr_data->arp_header->sender_ip,
				addr_data->ipv4_name[0], INET_ADDRSTRLEN));
	print_MAC_addr("\n\t", ' ', addr_data->arp_header->target_mac);
	fprintf(stdout," %s\n%s\n", inet_ntop(AF_INET, addr_data->arp_header->target_ip,
				addr_data->ipv4_name[1], INET_ADDRSTRLEN),
			"     +--------+--------+--------+--------+--------+--------+\n");
}
